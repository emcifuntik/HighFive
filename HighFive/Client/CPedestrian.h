#pragma once

class CPedestrian: public CEntity
{
public:
	CPedestrian(Ped handle);
	CPedestrian(Hash Model, CVector3 Position, float Heading);
	~CPedestrian();
	void SetDesiredHeading(float heading);
	float GetDesiredHeading() { return CEntity::GetHeading(); }
	void GetMoveSpeed(CVector3& vecMoveSpeed);
	void TaskMove(CVector3 vecPos, float fMoveSpeed);
	void ClearTasks();
	void SetCurrentHeading(float heading) { CEntity::SetHeading(heading); }
	float GetCurrentHeading() { return CEntity::GetHeading(); }
	void SetModel(Hash model);
	bool IsDucking();
	void SetDucking(bool ducking);
	bool IsJumping();
	void TaskJump();
	void SetCoordsKeepVehicle(float x, float y, float z);
	void SetArmour(unsigned short armour);
	unsigned short GetArmour();
	Hash GetCurrentWeapon();
	void SetCurrentWeapon(Hash weapon, bool equipNow = TRUE);
	unsigned int GetCurrentWeaponAmmo();
	unsigned int GetAmmo(Hash weapon);
	void SetAmmo(Hash weapon, unsigned int ammo);
	float GetBlendRation();
	void SetComponentVariation(int component, int drawable, int texture, int palette);
	void SetMoney(int money);
	void GiveMoney(int money);
	int GetMoney();
	void GiveWeapon(Hash weapon, int ammo);
	void RemoveWeapon(Hash weapon);
	void RemoveAllWeapons();
	void SetAmmo(Hash weapon, int ammo);
	void AddAmmo(Hash weapon, int ammo);
	void RemoveAmmo(Hash weapon, int ammo);
};
