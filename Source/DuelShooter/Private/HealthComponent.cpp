// All rights by IsGris


#include "HealthComponent.h"

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	PrimaryComponentTick.bCanEverTick = false;
	
	if ( auto Owner = GetOwner() )
	{
		Owner->OnTakeAnyDamage.AddDynamic( this, &UHealthComponent::GetAnyDamage );
	}
}

int UHealthComponent::GetHealth()
{
	return CurrentHealth;
}

void UHealthComponent::SetHealth(int NewValue)
{
	CurrentHealth = NewValue;
	CheckDeath();
}

void UHealthComponent::RecieveDamage(int Damage)
{
	SetHealth(GetHealth() - Damage);
}

void UHealthComponent::operator-=( int Subtrahend )
{
	RecieveDamage(Subtrahend);
}

void UHealthComponent::operator+=( int Term )
{
	SetHealth(GetHealth() + Term);
}

void UHealthComponent::operator*=( int Multiplier )
{
	SetHealth(GetHealth() * Multiplier);
}

void UHealthComponent::operator/=( int Divider )
{
	SetHealth(GetHealth() / Divider);
}

bool UHealthComponent::CheckDeath()
{
	if (GetHealth() < 0)
	{
		OnDeath.Broadcast();
		return true;
	}
	else
		return false;
}

void UHealthComponent::GetAnyDamage( AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser )
{
	*this -= Damage;
}
