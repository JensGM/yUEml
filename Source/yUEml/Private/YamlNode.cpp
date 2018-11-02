// Fill out your copyright notice in the Description page of Project Settings.

#include "YamlNode.h"
#include "Regex.h"


bool FYamlNode::TryGetMap(TMap<FString, TSharedRef<FYamlNode>>& Map) const
{
	return false;
}

bool FYamlNode::TryGetSequence(TArray<TSharedRef<FYamlNode>>& Seq) const
{
	return false;
}

bool FYamlNode::TryGetBool(bool& Value) const
{
	return false;
}

bool FYamlNode::TryGetFloat(float& Value) const
{
	return false;
}

bool FYamlNode::TryGetInt(int32& Value) const
{
	return false;
}

bool FYamlNode::TryGetString(FString& Value) const
{
	return false;
}

FYamlMap::FYamlMap(const TMap<FString, TSharedRef<FYamlNode>>& Map)
{
	Data = Map;
}

bool FYamlMap::TryGetMap(TMap<FString, TSharedRef<FYamlNode>>& Map) const
{
	Map = TMap<FString, TSharedRef<FYamlNode>>(Data);
	return true;
}

FYamlSequence::FYamlSequence(const TArray<TSharedRef<FYamlNode>>& Seq)
{
	Data = Seq;
}

bool FYamlSequence::TryGetSequence(TArray<TSharedRef<FYamlNode>>& Seq) const
{
	Seq = TArray<TSharedRef<FYamlNode>>(Data);
	return true;
}

FYamlScalar::FYamlScalar(const FString& Value)
{
	Data = Value;
}

bool FYamlScalar::TryGetBool(bool& Value) const
{
	static const FRegexPattern TruePattern(TEXT("^(?:y|Y|yes|Yes|YES|true|True|TRUE|on|On|ON)$"));
	static const FRegexPattern FalsePattern(TEXT("^(?:n|N|no|No|NO|false|False|FALSE|off|Off|OFF)$"));
	FRegexMatcher TrueMatcher(TruePattern, Data);
	FRegexMatcher FalseMatcher(FalsePattern, Data);

	if (TrueMatcher.FindNext())
	{
		Value = true;
		return true;
	}

	if (FalseMatcher.FindNext())
	{
		Value = false;
		return true;
	}

	return false;
}

bool FYamlScalar::TryGetFloat(float& Value) const
{
	if (FCString::IsNumeric(*Data))
	{
		Value = FCString::Atof(*Data);
		return true;
	}
	return false;
}

bool FYamlScalar::TryGetInt(int32& Value) const
{
	if (FCString::IsNumeric(*Data))
	{
		Value = FCString::Atoi(*Data);
		return true;
	}
	return false;
}

bool FYamlScalar::TryGetString(FString& Value) const
{
	Value = Data;
	return true;
}
