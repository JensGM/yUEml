// Fill out your copyright notice in the Description page of Project Settings.

#include "YamlFunctionLibrary.h"
#include <stdio.h>
#include "yaml/yaml.h"


namespace
{
	struct AutoYamlParser : public yaml_parser_t
	{
		AutoYamlParser(FILE* fp)
		{
			yaml_parser_initialize(this);
			yaml_parser_set_input_file(this, fp);
		}

		~AutoYamlParser()
		{
			yaml_parser_delete(this);
		}
	};

	struct AutoYamlEvent : public yaml_event_t
	{
		~AutoYamlEvent()
		{
			yaml_event_delete(this);
		}
	};

	const TSharedRef<AutoYamlEvent> NextYamlEvent(TSharedRef<AutoYamlParser> Parser)
	{
		TSharedRef<AutoYamlEvent> Event( new AutoYamlEvent );
		yaml_parser_parse(&Parser.Get(), &Event.Get());
		return Event;
	}

	TSharedPtr<FYamlNode> ParseYamlNode(const TSharedRef<AutoYamlEvent> Event, TSharedRef<AutoYamlParser> Parser);
	TSharedPtr<FYamlMap> ParseYamlMapping(TSharedRef<AutoYamlParser> Parser);
	TSharedPtr<FYamlSequence> ParseYamlSequence(TSharedRef<AutoYamlParser> Parser);

	TSharedPtr<FYamlNode> ParseYamlNode(const TSharedRef<AutoYamlEvent> Event, TSharedRef<AutoYamlParser> Parser)
	{
		TSharedPtr<FYamlNode> Node;

		switch (Event->type)
		{
		case YAML_STREAM_START_EVENT:
		case YAML_DOCUMENT_START_EVENT:
			// TODO Report unexpected token
			break;
		
		case YAML_STREAM_END_EVENT:
		case YAML_DOCUMENT_END_EVENT:
			// TODO Report unexpected end of document
			break;

		case YAML_SCALAR_EVENT:
		{
			FString Value = UTF8_TO_TCHAR(Event->data.scalar.value);
			Node = MakeShareable( new FYamlScalar(Value) );
			break;
		}
			
		case YAML_SEQUENCE_START_EVENT:
		{
			TSharedPtr<FYamlSequence> SequenceNode = ParseYamlSequence(Parser);
			if (SequenceNode)
			{
				Node = SequenceNode;
			}
			break;
		}

		case YAML_MAPPING_START_EVENT:
		{
			TSharedPtr<FYamlMap> MapNode = ParseYamlMapping(Parser);
			if (MapNode)
			{
				Node = MapNode;
			}
			break;
		}

		case YAML_SEQUENCE_END_EVENT:
		case YAML_MAPPING_END_EVENT:
			// TODO Report unexpeted end of data
			break;

		case YAML_ALIAS_EVENT:
			check(false && "Yaml aliasing not supported");
			break;

		default:
			check(false && "Yaml parsing encountered a fatal error.");
			break;

		}

		return Node;
	}

	TSharedPtr<FYamlMap> ParseYamlMapping(TSharedRef<AutoYamlParser> Parser)
	{
		bool bGood = true;
		TMap<FString, TSharedRef<FYamlNode>> Map;

		while (true)
		{
			auto Event = NextYamlEvent(Parser);

			if (Event->type == YAML_MAPPING_END_EVENT)
			{
				break;
			}

			auto KeyNode = ParseYamlNode(Event, Parser);
			FString Key;
			if (KeyNode && KeyNode->TryGetString(Key))
			{
				Event = NextYamlEvent(Parser);
				auto ValueNode = ParseYamlNode(Event, Parser);
				if (ValueNode)
				{
					Map.Add(Key, ValueNode.ToSharedRef());
				}
				else
				{
					// TODO Report parsing error
					bGood = false;
				}
			}
			else
			{
				// TODO Report parsing error
				bGood = false;
			}
		}

		if (bGood) return MakeShareable( new FYamlMap(Map) );
		else return TSharedPtr<FYamlMap>();
	}

	TSharedPtr<FYamlSequence> ParseYamlSequence(TSharedRef<AutoYamlParser> Parser)
	{
		bool bGood = true;
		TArray<TSharedRef<FYamlNode>> Seq;

		while (true)
		{
			auto Event = NextYamlEvent(Parser);

			if (Event->type == YAML_SEQUENCE_END_EVENT)
			{
				break;
			}

			auto Node = ParseYamlNode(Event, Parser);
			if (Node)
			{
				Seq.Add(Node.ToSharedRef());
			}
			else
			{
				bGood = false;
				break;
			}

		}

		if (bGood) return MakeShareable(new FYamlSequence(Seq));
		else return TSharedPtr<FYamlSequence>();
	}
}


TSharedPtr<FYamlNode> UYamlFunctionLibrary::LoadYaml(const FString& FilePath)
{
	TSharedPtr<FYamlNode> Node;

	FILE* fp;
	fopen_s(&fp, TCHAR_TO_ANSI(*FilePath), "r");

	if (fp)
	{
		TSharedRef<AutoYamlParser> Parser(new AutoYamlParser(fp));
		
		while (true)
		{
			auto Event = NextYamlEvent(Parser);

			if (Event->type == YAML_STREAM_END_EVENT)
				break;

			switch (Event->type)
			{
			case YAML_STREAM_START_EVENT:
			case YAML_DOCUMENT_START_EVENT:
			case YAML_DOCUMENT_END_EVENT:
				break;
				
			default:
			{
				if (!Node)
				{
					Node = ParseYamlNode(Event, Parser);
				}
				else
				{
					// TODO Report multiple roots
					return TSharedPtr<FYamlNode>();
				}
			}
			}
		}
	}
	else
	{
		// TODO Report failure to open file
	}

	fclose(fp);

	return Node;
}
