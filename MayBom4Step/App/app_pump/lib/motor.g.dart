// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'motor.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

Motor _$MotorFromJson(Map<String, dynamic> json) => Motor(
      json['noM'] as int,
      (json['pulse'] as num).toDouble(),
      (json['microStep'] as num).toDouble(),
      (json['speed'] as num).toDouble(),
      json['setTime'] as int,
    );

Map<String, dynamic> _$MotorToJson(Motor instance) => <String, dynamic>{
      'noM': instance.noM,
      'pulse': instance.pulse,
      'microStep': instance.microStep,
      'speed': instance.speed,
      'setTime': instance.setTime,
    };
