import 'dart:convert';

import 'package:app_pump/motor.dart';
import 'package:flutter/material.dart';
import 'package:web_socket_channel/io.dart';
import 'package:web_socket_channel/status.dart' as status;
import './motor.dart';
import './runData.dart';
import './settingData.dart';
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
  String msg = '';
  List<Motor> motor_list = [
    Motor(1, 101, 1001, 1),
    Motor(2, 202, 2002, 2),
    Motor(3, 303, 3003, 3),
    Motor(4, 404, 4004, 4)
  ];
  late SettingData settingData;
  late RunData runData;

  double btWidth = 40;
  double btHeigh = 40;
  double textWidth = 120;

  @override
  void initState() {
    runData = RunData("run", [0, 0, 0, 0]);
    settingData = SettingData("setting", motor_list);
    connected = false; //initially connection status is "NO" so its FALSE
    isLoaded = false;
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
            if (json.containsValue("run")) {
              var getData = RunData.fromJson(json);
              setState(() {
                isLoaded = true;
                runData = getData;
              });
            }
            if (json.containsValue("set")) {
              var getData = SettingData.fromJson(json);
              setState(() {
                isLoaded = true;
                settingData = getData;
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

  void sendToServer(SettingData data) {
    String s = jsonEncode(data);
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
          IconButton(
            onPressed: () {
              connected ? reloadData() : null;
            },
            icon: Icon(
              Icons.refresh,
              color: isLoaded ? Colors.white : Colors.white54,
            ),
          ),
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
            Card(
              child: Padding(
                padding: const EdgeInsets.all(6.0),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceBetween,
                  children: [
                    ElevatedButton(
                      onPressed: () {
                        settingData.type = "set";
                        sendToServer(settingData);
                      },
                      style: ButtonStyle(
                        overlayColor:
                            MaterialStateProperty.all(Colors.orangeAccent),
                        padding:
                            MaterialStateProperty.all(const EdgeInsets.all(15)),
                        fixedSize:
                            MaterialStateProperty.all(const Size(100, 60)),
                      ),
                      child: Text('Save',
                          style: TextStyle(
                              fontSize: 24,
                              fontWeight: FontWeight.bold,
                              color: Colors.white)),
                    ),
                  ],
                ),
              ),
            ),
            motorItem(context, settingData.motors[0], runData.runTime[0]),
            motorItem(context, settingData.motors[1], runData.runTime[1]),
            motorItem(context, settingData.motors[2], runData.runTime[2]),
            motorItem(context, settingData.motors[3], runData.runTime[3]),
            Padding(padding: EdgeInsets.only(top: 80))
          ],
        ),
      ),
    );
  }

  Widget motorItem(BuildContext context, Motor motor, double time) {
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
                    decoration: InputDecoration(
                        border: OutlineInputBorder(), labelText: 'Số xung'),
                    keyboardType: TextInputType.number,
                    autocorrect: true,
                    style: TextStyle(fontSize: textFontSize),
                    textAlign: TextAlign.center,
                    textAlignVertical: TextAlignVertical.center,
                    controller: TextEditingController()
                      ..text = motor.pulse.toStringAsFixed(0),
                    onChanged: (String value) {
                      if (value.length > 0) {
                        motor.pulse = double.parse(value);
                      }
                    },
                  ),
                ),
                Padding(padding: EdgeInsets.all(4)),
                Expanded(
                  child: TextField(
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
