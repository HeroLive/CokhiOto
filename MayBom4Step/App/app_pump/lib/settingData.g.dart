// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'settingData.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

SettingData _$SettingDataFromJson(Map<String, dynamic> json) => SettingData(
      json['type'] as String,
      (json['motors'] as List<dynamic>)
          .map((e) => Motor.fromJson(e as Map<String, dynamic>))
          .toList(),
    );

Map<String, dynamic> _$SettingDataToJson(SettingData instance) =>
    <String, dynamic>{
      'type': instance.type,
      'motors': instance.motors,
    };
