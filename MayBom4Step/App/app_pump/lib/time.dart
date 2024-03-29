import 'package:json_annotation/json_annotation.dart';
part 'time.g.dart';

@JsonSerializable()
class Time {
  int noM;
  int h;
  int m;
  int s;

  Time(this.noM, this.h, this.m, this.s);

  factory Time.fromJson(Map<String, dynamic> json) => _$TimeFromJson(json);
  Map<String, dynamic> toJson() => _$TimeToJson(this);
}
