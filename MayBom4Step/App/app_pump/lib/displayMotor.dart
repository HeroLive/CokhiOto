import 'package:json_annotation/json_annotation.dart';
part 'displayMotor.g.dart';

@JsonSerializable()
class DisplayMotor {
  int noM;
  double volumn;
  double microStep;
  double speed;
  double setTime;

  DisplayMotor(this.noM, this.volumn, this.microStep, this.speed, this.setTime);

  factory DisplayMotor.fromJson(Map<String, dynamic> json) =>
      _$DisplayMotorFromJson(json);
  Map<String, dynamic> toJson() => _$DisplayMotorToJson(this);
}
