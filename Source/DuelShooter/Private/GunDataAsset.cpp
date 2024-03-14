// All rights by IsGris


#include "GunDataAsset.h"
#include "GenericPlatform/GenericPlatformMath.h"

void FGunConsumables::InitGun( const FGunInfo& GunInfo )
{
	Gun = &GunInfo;
	BulletsInMagazineRemains = GunInfo.MagazineAmmoCount;
	BulletsNotInMagazineRemains = FGenericPlatformMath::Max(0, GunInfo.AmmoOnStart - GunInfo.MagazineAmmoCount);
	CurrentBulletShotedContinouslyCount = 0;
}

FRotator FGunConsumables::GetRotationToAppendForSpread()
{
	if ( !Gun->Spread.IsEmpty() )
		return FRotator( 0, Gun->Spread[CurrentBulletShotedContinouslyCount].Key, Gun->Spread[CurrentBulletShotedContinouslyCount].Value );
	else
		return FRotator();
}

void FGunInfo::Init()
{
	if ( MagazineAmmoCount > 0 && SpreadX.GetRichCurveConst()->GetNumKeys() > 0 && SpreadY.GetRichCurveConst()->GetNumKeys() > 0 )
	{
		Spread.Empty();
		Spread.Reserve( MagazineAmmoCount );
		IntervalsBetweenShotsInCurveX = ( SpreadX.GetRichCurveConst()->GetLastKey().Time - SpreadX.GetRichCurveConst()->GetFirstKey().Time ) / MagazineAmmoCount;
		IntervalsBetweenShotsInCurveY = ( SpreadY.GetRichCurveConst()->GetLastKey().Time - SpreadY.GetRichCurveConst()->GetFirstKey().Time ) / MagazineAmmoCount;
		for ( int i{}; i < MagazineAmmoCount; i++ )
		{
			Spread.Add( { SpreadX.GetRichCurveConst()->Eval( i * IntervalsBetweenShotsInCurveX ), SpreadY.GetRichCurveConst()->Eval( i * IntervalsBetweenShotsInCurveY ) } );
		}
	}
}

void UGunDataAsset::Init()
{
	bIsInited = true;
	for ( auto& GunInfo : Guns )
		GunInfo.Init();
}
