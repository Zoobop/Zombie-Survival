// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/SurvivalMode.h"
#include "Entities/Soldier.h"

ASurvivalMode::ASurvivalMode()
{
	Instance = this;
}

void ASurvivalMode::UndeadDied(class AUndead* Killed, class ASoldier* Killer)
{
	if (Killed) {

	}
}

void ASurvivalMode::PlayerDied(class ASoldier* Killed)
{
	if (Killed) {

		Survivors.RemoveSingle(Killed);

	}
}

void ASurvivalMode::BeginPlay()
{

}
