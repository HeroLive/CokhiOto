// ignore: file_names
import 'package:json_annotation/json_annotation.dart';
import './displayMotor.dart';
import './time.dart';
part 'displaySettingData.g.dart';

@JsonSerializable()
class DisplaySettingData {
  String type;
  List<DisplayMotor> motors;

  DisplaySettingData(this.type, this.motors);

  factory DisplaySettingData.fromJson(Map<String, dynamic> json) =>
      _$DisplaySettingDataFromJson(json);
  Map<String, dynamic> toJson() => _$DisplaySettingDataToJson(this);
}
