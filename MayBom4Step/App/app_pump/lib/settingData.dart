// ignore: file_names
import 'package:json_annotation/json_annotation.dart';
import './motor.dart';
import './time.dart';
part 'settingData.g.dart';

@JsonSerializable()
class SettingData {
  String type;
  List<Motor> motors;

  SettingData(this.type, this.motors);

  factory SettingData.fromJson(Map<String, dynamic> json) =>
      _$SettingDataFromJson(json);
  Map<String, dynamic> toJson() => _$SettingDataToJson(this);
}
