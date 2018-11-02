// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/**
 * 
 */
class YUEML_API FYamlNode
{
	friend class UYamlFunctionLibrary;
public:
	virtual ~FYamlNode() = default;

	virtual bool TryGetMap(TMap<FString, TSharedRef<FYamlNode>>& Map) const;
	virtual bool TryGetSequence(TArray<TSharedRef<FYamlNode>>& Seq) const;
	virtual bool TryGetBool(bool& Value) const;
	virtual bool TryGetFloat(float& Value) const;
	virtual bool TryGetInt(int32& Value) const;
	virtual bool TryGetString(FString& Value) const;

protected:
	FYamlNode() = default;
};

class YUEML_API FYamlMap : public FYamlNode
{
public:
	FYamlMap(const TMap<FString, TSharedRef<FYamlNode>>& Map);
	bool TryGetMap(TMap<FString, TSharedRef<FYamlNode>>& Map) const override;

private:
	TMap<FString, TSharedRef<FYamlNode>> Data;
};

class YUEML_API FYamlSequence : public FYamlNode
{
public:
	FYamlSequence(const TArray<TSharedRef<FYamlNode>>& Seq);
	bool TryGetSequence(TArray<TSharedRef<FYamlNode>>& Seq) const override;

private:
	TArray<TSharedRef<FYamlNode>> Data;
};

class YUEML_API FYamlScalar : public FYamlNode
{
public:
	FYamlScalar(const FString& Value);
	bool TryGetBool(bool& Value) const override;
	bool TryGetFloat(float& Value) const override;
	bool TryGetInt(int32& Value) const override;
	bool TryGetString(FString& Value) const override;

private:
	FString Data;
};
