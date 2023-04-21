// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'displayMotor.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

DisplayMotor _$DisplayMotorFromJson(Map<String, dynamic> json) => DisplayMotor(
      json['noM'] as int,
      (json['volumn'] as num).toDouble(),
      (json['microStep'] as num).toDouble(),
      (json['speed'] as num).toDouble(),
      (json['setTime'] as num).toDouble(),
    );

Map<String, dynamic> _$DisplayMotorToJson(DisplayMotor instance) =>
    <String, dynamic>{
      'noM': instance.noM,
      'volumn': instance.volumn,
      'microStep': instance.microStep,
      'speed': instance.speed,
      'setTime': instance.setTime,
    };
