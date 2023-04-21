// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'time.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

Time _$TimeFromJson(Map<String, dynamic> json) => Time(
      json['noM'] as int,
      json['h'] as int,
      json['m'] as int,
      json['s'] as int,
    );

Map<String, dynamic> _$TimeToJson(Time instance) => <String, dynamic>{
      'noM': instance.noM,
      'h': instance.h,
      'm': instance.m,
      's': instance.s,
    };
