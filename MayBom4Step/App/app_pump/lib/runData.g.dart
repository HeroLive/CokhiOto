// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'runData.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

RunData _$RunDataFromJson(Map<String, dynamic> json) => RunData(
      json['type'] as String,
      (json['runTime'] as List<dynamic>)
          .map((e) => (e as num).toDouble())
          .toList(),
    );

Map<String, dynamic> _$RunDataToJson(RunData instance) => <String, dynamic>{
      'type': instance.type,
      'runTime': instance.runTime,
    };
