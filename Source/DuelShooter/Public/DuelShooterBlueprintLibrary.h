// All rights by IsGris

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DuelShooterBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DUELSHOOTER_API UDuelShooterBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Returns pointer to duel shooter game mode
	UFUNCTION( BlueprintPure, Category = "Game", meta = ( WorldContext = "WorldContextObject" ) )
	static class ADuelShooterGameMode* GetShooterGameMode( const UObject* WorldContextObject );

	// ~ Search
	// ~ ~ Actors
public:
	/*
	* Finds actor with given type in world
	* @param ActorNumber Which actor by serial number starting from 0 are we interested in
	*/
	template<class ActorType UE_REQUIRES( TIsDerivedFrom<ActorType, AActor>::Value )>
	static TOptional<AActor*> GetActorOfType( const UObject* WorldContextObject, const uint16 ActorNumber );
	/*
	* Finds actor with given type in world
	* @param ActorType Type of actor we looking for
	* @param ActorNumber Which actor by serial number starting from 0 are we interested in
	*/
	static TOptional<AActor*> GetActorOfType( const UObject* WorldContextObject, TSubclassOf<AActor> ActorType, const uint16 ActorNumber );
	/*
	* Finds actor with given type in world
	* @param ActorType Type of actor we looking for
	* @param ActorNumber Which actor by serial number starting from 0 are we interested in
	*/
	UFUNCTION( BlueprintPure, Category = "Search", meta = ( WorldContext = "WorldContextObject", ReturnDisplayName = "Finded actor", KeyWords = "Search Find", ExpandBoolAsExecs = "FindSucceed" ) )
	static AActor* GetActorOfType( const UObject* WorldContextObject, TSubclassOf<AActor> ActorType, bool& FindSucceed, const int ActorNumber );
	/*
	* Finds start Transform in level that has type StartType
	* @param StartNumber Which start by serial number starting from 0 are we interested in
	*/
	template<class StartType UE_REQUIRES( TIsDerivedFrom<StartType, AActor>::Value )>
	static TOptional<FTransform> FindStartTransform( const UObject* WorldContextObject, const uint16 StartNumber );
	/*
	* Finds start Transform in level that has type StartType
	* @param StartType Type of start that we looking for
	* @param StartNumber Which start by serial number starting from 0 are we interested in
	*/
	static TOptional<FTransform> FindStartTransform( const UObject* WorldContextObject, TSubclassOf<AActor> StartType, const uint16 StartNumber );
	/*
	* Finds start Transform in level that has type StartType
	* @param StartType Type of start that we looking for
	* @param StartNumber Which start by serial number starting from 0 are we interested in
	*/
	UFUNCTION( BlueprintPure, meta = ( WorldContext = "WorldContextObject", KeyWords = "Search Find", ExpandBoolAsExecs = "FindSucceed" ) )
	static FTransform FindStartTransform( const UObject* WorldContextObject, TSubclassOf<AActor> StartType, bool& FindSucceed, const int StartNumber );
	// ~ ~ Actors
	// ~ Search
};
