// All rights by IsGris

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeath;

	// Get current health value
	virtual int GetHealth();
	// Sets new value for health
	virtual void SetHealth( int NewValue );

	/*
	* Recieve damage for health
	* @param Damage How many damage health recieve
	*/
	virtual void RecieveDamage( int Damage );

	void operator-=( int Subtrahend );
	void operator+=( int Term );
	void operator*=( int Multiplier );
	void operator/=( int Divider );

protected:
	// Check does health below zero if so broadcast event death
	bool CheckDeath();

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Health" )
	int DefaultHealth;
private:
	UPROPERTY( EditAnywhere, Category = "Health" )
	int MaxHealth;
	UPROPERTY( EditInstanceOnly, Category = "Health" )
	int CurrentHealth;

private:
	UFUNCTION()
	void GetAnyDamage( AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser );
};
