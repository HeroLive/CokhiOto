// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'motor.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

Motor _$MotorFromJson(Map<String, dynamic> json) => Motor(
      json['noM'] as int,
      (json['pulse'] as num).toDouble(),
      (json['speed'] as num).toDouble(),
      (json['setTime'] as num).toDouble(),
    );

Map<String, dynamic> _$MotorToJson(Motor instance) => <String, dynamic>{
      'noM': instance.noM,
      'pulse': instance.pulse,
      'speed': instance.speed,
      'setTime': instance.setTime,
    };
