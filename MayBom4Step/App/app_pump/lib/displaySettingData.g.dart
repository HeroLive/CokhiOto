// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'displaySettingData.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

DisplaySettingData _$DisplaySettingDataFromJson(Map<String, dynamic> json) =>
    DisplaySettingData(
      json['type'] as String,
      (json['motors'] as List<dynamic>)
          .map((e) => DisplayMotor.fromJson(e as Map<String, dynamic>))
          .toList(),
    );

Map<String, dynamic> _$DisplaySettingDataToJson(DisplaySettingData instance) =>
    <String, dynamic>{
      'type': instance.type,
      'motors': instance.motors,
    };
