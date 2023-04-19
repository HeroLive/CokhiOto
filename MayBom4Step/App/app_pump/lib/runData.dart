// ignore: file_names
import 'package:json_annotation/json_annotation.dart';
import './motor.dart';
import './time.dart';
part 'runData.g.dart';

@JsonSerializable()
class RunData {
  String type;
  List<double> runTime;

  RunData(this.type, this.runTime);

  factory RunData.fromJson(Map<String, dynamic> json) =>
      _$RunDataFromJson(json);
  Map<String, dynamic> toJson() => _$RunDataToJson(this);
}
