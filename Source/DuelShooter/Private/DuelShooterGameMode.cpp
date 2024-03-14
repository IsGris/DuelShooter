// All rights by IsGris


#include "DuelShooterGameMode.h"
#include "GunDataAsset.h"

DEFINE_LOG_CATEGORY( DuelShooterGameModeLog );

void ADuelShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = false;

	GunsDatas = GunsDatas.LoadSynchronous();
	if ( GunsDatas )
	{
		GunsDatas->Init();
		UE_LOG( DuelShooterGameModeLog, Log, TEXT( "Guns datas loaded" ) );
	} else
		UE_LOG( DuelShooterGameModeLog, Warning, TEXT( "Guns datas not defined" ) );

}

FGunInfo* ADuelShooterGameMode::GetGunData( const int GunId ) const
{
	if ( !GunsDatas )
		return nullptr;
	if ( GunId < GunsDatas.Get()->Guns.Num() )
	{
		return &GunsDatas.Get()->Guns[GunId];
	}

	return nullptr;
}
UGunDataAsset* ADuelShooterGameMode::GetGunDataAsset() const
{
	if ( !GunsDatas )
		return nullptr;
	else
		return GunsDatas.Get();
}
