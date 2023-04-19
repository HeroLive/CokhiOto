import 'package:json_annotation/json_annotation.dart';
part 'motor.g.dart';

@JsonSerializable()
class Motor {
  int noM;
  double pulse;
  double speed;
  double setTime;

  Motor(this.noM, this.pulse, this.speed, this.setTime);

  factory Motor.fromJson(Map<String, dynamic> json) => _$MotorFromJson(json);
  Map<String, dynamic> toJson() => _$MotorToJson(this);
}
