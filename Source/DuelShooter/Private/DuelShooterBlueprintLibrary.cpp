// All rights by IsGris


#include "DuelShooterBlueprintLibrary.h"
#include "DuelShooterGameMode.h"
#include "Kismet/GameplayStatics.h"

ADuelShooterGameMode* UDuelShooterBlueprintLibrary::GetShooterGameMode( const UObject* WorldContextObject )
{
	UWorld* const World = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::LogAndReturnNull );
	return World ? Cast<ADuelShooterGameMode>(World->GetAuthGameMode()) : NULL;
}

template<class ActorType UE_REQUIRES( TIsDerivedFrom<ActorType, AActor>::Value )>
static TOptional<AActor*> UDuelShooterBlueprintLibrary::GetActorOfType( const UObject* WorldContextObject, const uint16 ActorNumber )
{
	return GetActorOfType( WorldContextObject, ActorType::StaticClass(), ActorNumber );
}

TOptional<AActor*> UDuelShooterBlueprintLibrary::GetActorOfType( const UObject* WorldContextObject, TSubclassOf<AActor> ActorType, const uint16 ActorNumber )
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass( WorldContextObject, ActorType, OutActors );
	if ( OutActors.Num() <= ActorNumber || ActorNumber < 0 )
		return nullptr;
	return OutActors[ActorNumber];
}

AActor* UDuelShooterBlueprintLibrary::GetActorOfType( const UObject* WorldContextObject, TSubclassOf<AActor> ActorType, bool& FindSucceed, const int ActorNumber )
{
	TOptional<AActor*> result = GetActorOfType( WorldContextObject, ActorType, ActorNumber );
	if ( !result.IsSet() )
	{
		FindSucceed = false;
		return nullptr;
	}
	FindSucceed = true;
	return result.GetValue();
}

template<class StartType UE_REQUIRES( TIsDerivedFrom<StartType, AActor>::Value )>
static TOptional<FTransform> UDuelShooterBlueprintLibrary::FindStartTransform( const UObject* WorldContextObject, const uint16 StartNumber )
{
	return FindStartTransform( StartType::StaticClass(), StartNumber );
}

TOptional<FTransform> UDuelShooterBlueprintLibrary::FindStartTransform( const UObject* WorldContextObject, TSubclassOf<AActor> StartType, const uint16 StartNumber )
{
	TArray<AActor*> AllStartsOfType;
	UGameplayStatics::GetAllActorsOfClass( WorldContextObject, StartType, AllStartsOfType );

	if ( AllStartsOfType.Num() <= StartNumber || StartNumber < 0 )
		return TOptional<FTransform>();

	return AllStartsOfType[StartNumber]->GetTransform();
}

FTransform UDuelShooterBlueprintLibrary::FindStartTransform( const UObject* WorldContextObject, TSubclassOf<AActor> StartType, bool& FindSucceed, const int StartNumber )
{
	TOptional<FTransform> result = FindStartTransform( WorldContextObject, StartType, StartNumber );
	if ( !result.IsSet() )
	{
		FindSucceed = false;
		return FTransform();
	}
	FindSucceed = true;
	return result.GetValue();
}
