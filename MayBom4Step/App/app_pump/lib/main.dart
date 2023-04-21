import 'dart:convert';

import 'package:app_pump/displaySettingData.dart';
import 'package:app_pump/motor.dart';
import 'package:flutter/material.dart';
import 'package:web_socket_channel/io.dart';
import 'package:web_socket_channel/status.dart' as status;
import './motor.dart';
import './displayMotor.dart';
import './runData.dart';
import './settingData.dart';
import './displaySettingData.dart';
import './time.dart';

const String esp_url = 'ws://192.168.98.100:81';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        // This is the theme of your application.
        //
        // Try running your application with "flutter run". You'll see the
        // application has a blue toolbar. Then, without quitting the app, try
        // changing the primarySwatch below to Colors.green and then invoke
        // "hot reload" (press "r" in the console where you ran "flutter run",
        // or simply save your changes to "hot reload" in a Flutter IDE).
        // Notice that the counter didn't reset back to zero; the application
        // is not restarted.
        primarySwatch: Colors.blue,
      ),
      home: const MyHomePage(title: 'Điều khiển motor 4 trục'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({Key? key, required this.title}) : super(key: key);

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  late IOWebSocketChannel channel;
  late bool connected; //boolean value to track if WebSocket is connected
  late bool isLoaded;
  late bool isSetting;
  String msg = '';
  List<Motor> motor_list = [
    Motor(1, 101, 1, 1001, 1),
    Motor(2, 202, 1, 2002, 2),
    Motor(3, 303, 1, 3003, 3),
    Motor(4, 404, 1, 4004, 4)
  ];
  List<DisplayMotor> display_motor_list = [
    DisplayMotor(1, 101, 1, 1001, 1),
    DisplayMotor(2, 202, 1, 2002, 2),
    DisplayMotor(3, 303, 1, 3003, 3),
    DisplayMotor(4, 404, 1, 4004, 4)
  ];
  late SettingData settingData;
  late DisplaySettingData displaySettingData;
  late RunData runData;

  double btWidth = 40;
  double btHeigh = 40;
  double textWidth = 120;

  @override
  void initState() {
    runData = RunData("run", [0, 0, 0, 0]);
    displaySettingData = DisplaySettingData("set", display_motor_list);
    settingData = SettingData("set", motor_list);
    connected = false; //initially connection status is "NO" so its FALSE
    isLoaded = false;
    isSetting = false;
    Future.delayed(Duration.zero, () async {
      channelconnect(); //connect to WebSocket wth NodeMCU
    });
    super.initState();
  }

  channelconnect() {
    //function to connect
    try {
      channel = IOWebSocketChannel.connect(esp_url); //channel IP : Port
      channel.stream.listen(
        (message) {
          print('Received from MCU: $message');
          if (message == "connected") {
            print('Status from MCU: $message');
            setState(() {
              connected = true;
            });
          } else {
            Map<String, dynamic> json = jsonDecode(message);
            if (json.containsValue("run") && !isSetting) {
              var getData = RunData.fromJson(json);
              runData = getData;
              for (var i = 0; i < runData.runTime.length; i++) {
                runData.runTime[i] = runData.runTime[i] / 1000;
              }
              setState(() {
                isLoaded = true;
                runData;
              });
            }
            if (json.containsValue("set") && !isSetting) {
              var getData = SettingData.fromJson(json);
              settingData = getData;
              for (var i = 0; i < settingData.motors.length; i++) {
                displaySettingData.motors[i].setTime =
                    settingData.motors[i].setTime / 1000;
                displaySettingData.motors[i].volumn =
                    settingData.motors[i].pulse /
                        settingData.motors[i].microStep;
                displaySettingData.motors[i].speed =
                    settingData.motors[i].speed;
              }
              setState(() {
                isLoaded = true;
                displaySettingData;
              });
            }
          }
          //channel.sink.close(status.goingAway);
        },
        onDone: () {
          //if WebSocket is disconnected
          print("Web socket is closed");
          setState(() {
            msg = 'disconnected';
            isLoaded = false;
            connected = false;
          });
        },
        onError: (error) {
          print(error.toString());
        },
      );
    } catch (_) {
      print("error on connecting to websocket.");
    }
  }

  void reloadData() {
    String s = '{"type":"get"}';
    print(s);
    try {
      channel.sink.add(s);
    } catch (e) {
      print(e);
    }
  }

//{"type":"set","motors":[{"noM":1,"pulse":6400.0,"speed":20000.0,"setTime":1000.0},{"noM":2,"pulse":2222.0,"speed":2002.0,"setTime":20000.0},{"noM":3,"pulse":3333.0,"speed":3003.0,"setTime":30000.0},{"noM":4,"pulse":4444.0,"speed":4004.0,"setTime":40000.0}]}
  void sendToServer(DisplaySettingData data) {
    for (var i = 0; i < data.motors.length; i++) {
      settingData.motors[i].setTime = data.motors[i].setTime * 1000;
      settingData.motors[i].pulse =
          data.motors[i].volumn * data.motors[i].microStep;
      settingData.motors[i].speed = data.motors[i].speed;
      settingData.motors[i].microStep = data.motors[i].microStep;
    }
    String s = jsonEncode(settingData);
    print(s);
    if (s.isNotEmpty) {
      try {
        channel.sink.add(s);
      } catch (e) {
        print(e);
      }
    }
  }

  void channeldisconnect() {
    try {
      channel.sink.close();
    } catch (e) {
      print(e);
    }
  }

  @override
  void dispose() {
    channel.sink.close();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.title),
        actions: <Widget>[
          if (isSetting) ...[
            IconButton(
              onPressed: () {
                setState(() {
                  isSetting = false;
                });
              },
              icon: Icon(
                Icons.cancel,
                color: connected ? Colors.white : Colors.white54,
              ),
            ),
            IconButton(
              onPressed: () {
                sendToServer(displaySettingData);
                setState(() {
                  isSetting = false;
                });
              },
              icon: Icon(
                Icons.save,
                color: connected ? Colors.white : Colors.white54,
              ),
            )
          ] else ...[
            IconButton(
              onPressed: () {
                connected
                    ? (setState(() {
                        isSetting = true;
                      }))
                    : null;
              },
              icon: Icon(
                Icons.settings,
                color: connected ? Colors.white : Colors.white54,
              ),
            ),
            IconButton(
              onPressed: () {
                sendToServer(displaySettingData);
                setState(() {
                  isSetting = false;
                });
              },
              icon: Icon(
                Icons.save,
                color: connected ? Colors.white : Colors.white54,
              ),
            )
          ],
          IconButton(
              onPressed: () {
                connected ? channeldisconnect() : channelconnect();
              },
              icon: Icon(
                Icons.cast_connected,
                color: connected ? Colors.white : Colors.white54,
              )),
        ],
      ),
      body: SingleChildScrollView(
        child: Column(
          children: [
            motorItem(
                context, displaySettingData.motors[0], runData.runTime[0]),
            motorItem(
                context, displaySettingData.motors[1], runData.runTime[1]),
            motorItem(
                context, displaySettingData.motors[2], runData.runTime[2]),
            motorItem(
                context, displaySettingData.motors[3], runData.runTime[3]),
            Padding(padding: EdgeInsets.only(top: 80))
          ],
        ),
      ),
    );
  }

  Widget motorItem(BuildContext context, DisplayMotor motor, double time) {
    double tfWidth = 200;
    double textFontSize = 24;
    return Card(
      elevation: 3,
      child: Padding(
        padding: const EdgeInsets.fromLTRB(4, 8, 3, 10),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              "Motor ${motor.noM}",
              style: TextStyle(
                  fontSize: 18,
                  fontWeight: FontWeight.bold,
                  color: Colors.redAccent),
            ),
            Padding(padding: EdgeInsets.all(10)),
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Expanded(
                  child: TextField(
                    enabled: isSetting ? true : false,
                    decoration: InputDecoration(
                        border: OutlineInputBorder(), labelText: 'Dung tích'),
                    keyboardType: TextInputType.number,
                    autocorrect: true,
                    style: TextStyle(fontSize: textFontSize),
                    textAlign: TextAlign.center,
                    textAlignVertical: TextAlignVertical.center,
                    controller: TextEditingController()
                      ..text = motor.volumn.toStringAsFixed(2),
                    onChanged: (String value) {
                      if (value.length > 0) {
                        motor.volumn = double.parse(value);
                      }
                    },
                  ),
                ),
                Expanded(
                  child: TextField(
                    enabled: isSetting ? true : false,
                    decoration: InputDecoration(
                        border: OutlineInputBorder(), labelText: 'Vi bước'),
                    keyboardType: TextInputType.number,
                    autocorrect: true,
                    style: TextStyle(fontSize: textFontSize),
                    textAlign: TextAlign.center,
                    textAlignVertical: TextAlignVertical.center,
                    controller: TextEditingController()
                      ..text = motor.microStep.toStringAsFixed(0),
                    onChanged: (String value) {
                      if (value.length > 0) {
                        motor.microStep = double.parse(value);
                      }
                    },
                  ),
                ),
                Padding(padding: EdgeInsets.all(4)),
                Expanded(
                  child: TextField(
                    enabled: isSetting ? true : false,
                    decoration: InputDecoration(
                        border: OutlineInputBorder(), labelText: 'Tốc độ'),
                    keyboardType: TextInputType.number,
                    style: TextStyle(fontSize: textFontSize),
                    textAlign: TextAlign.center,
                    textAlignVertical: TextAlignVertical.center,
                    controller: TextEditingController()
                      ..text = motor.speed.toStringAsFixed(0),
                    onChanged: (String value) {
                      if (value.length > 0) {
                        var _speed = double.parse(value);
                        motor.speed = _speed;
                      }
                    },
                  ),
                ),
              ],
            ),
            Padding(padding: EdgeInsets.all(10)),
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Expanded(
                  child: TextField(
                    enabled: isSetting ? true : false,
                    decoration: InputDecoration(
                        border: OutlineInputBorder(),
                        labelText: 'Thời gian cài đặt'),
                    keyboardType: TextInputType.number,
                    style: TextStyle(fontSize: textFontSize),
                    textAlign: TextAlign.center,
                    textAlignVertical: TextAlignVertical.center,
                    controller: TextEditingController()
                      ..text = motor.setTime.toStringAsFixed(0),
                    onChanged: (String value) {
                      if (value.length > 0) {
                        var _time = double.parse(value);
                        motor.setTime = _time;
                      }
                    },
                  ),
                ),
                Padding(padding: EdgeInsets.all(4)),
                Expanded(
                  child: TextField(
                      enabled: false,
                      decoration: InputDecoration(
                          border: OutlineInputBorder(),
                          labelText: 'Thời gian chạy'),
                      keyboardType: TextInputType.number,
                      style: TextStyle(fontSize: textFontSize),
                      textAlign: TextAlign.center,
                      textAlignVertical: TextAlignVertical.center,
                      controller: TextEditingController()
                        ..text = time.toStringAsFixed(0)),
                ),
              ],
            )
          ],
        ),
      ),
    );
  }
}
