// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'time.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

Time _$TimeFromJson(Map<String, dynamic> json) => Time(
      json['noM'] as int,
      (json['h'] as num).toDouble(),
      (json['m'] as num).toDouble(),
      (json['s'] as num).toDouble(),
    );

Map<String, dynamic> _$TimeToJson(Time instance) => <String, dynamic>{
      'noM': instance.noM,
      'h': instance.h,
      'm': instance.m,
      's': instance.s,
    };
