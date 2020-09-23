#include "Sophia.h"

#include "Game.h"
#include "Utils.h"

#include <algorithm>



CSophia::CSophia(float x, float y)
{

	level = 1;
	untouchable = 0;
	SetState(SOPHIA_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

}

void CSophia::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += SOPHIA_GRAVITY *dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SOPHIA_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SOPHIA_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		if (ny < 0)
		{
			isJump = false;
		}

		
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSophia::Render()
{

	int alpha = 255;
	if (untouchable) alpha = 128;

	if (state == SOPHIA_STATE_IDLE && vx == 0)
	{
		if (nx>0)
			ani = SOPHIA_ANI_WALKING_RIGHT;
		else if(nx < 0)
			ani = SOPHIA_ANI_WALKING_LEFT;
				
		if (currentframe == 0 || currentframe == 2 || currentframe == 4 || currentframe == 6)
			currentframe++;
		//DebugOut(L"\ncurrent:  %d", currentframe);
		animation_set->at(ani)->RenderIdle(x, y, currentframe);
	}
	else
	{
		if (state == SOPHIA_STATE_JUMP)
		{
    		if (vx == 0)
			{
				if (vy < 0)
				{
					if (nx > 0)
						ani = SOPHIA_ANI_JASON_JUMP_UP_IDLE_RIGHT;
					else
						ani = SOPHIA_ANI_JASON_JUMP_UP_IDLE_LEFT;
				}
				else if (vy > 0)
				{
					if (nx > 0)
						ani = SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_RIGHT;
					else
						ani = SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_LEFT;
				}
				
			}
			else
			{
				if (vy < 0)
				{
					if (nx > 0)
						ani = SOPHIA_ANI_JASON_JUMP_UP_WALKING_RIGHT;
					else
						ani = SOPHIA_ANI_JASON_JUMP_UP_WALKING_LEFT;
				}
				else if (vy > 0)
				{
					if (nx > 0)

						ani = SOPHIA_ANI_JASON_JUMP_DOWN_WALKING_RIGHT;
					else
						ani = SOPHIA_ANI_JASON_JUMP_DOWN_WALKING_LEFT;

					//currentframe = animation_set->at(ani)->currentFrame;
				}
				
			}
			
		}
		else if (state == SOPHIA_STATE_WALKING_RIGHT)
		{
			ani = SOPHIA_ANI_WALKING_RIGHT;
			currentframe = animation_set->at(ani)->currentFrame;
			
		}
		else if (state == SOPHIA_STATE_WALKING_LEFT)
		{
			ani = SOPHIA_ANI_WALKING_LEFT;
			currentframe = animation_set->at(ani)->currentFrame;
			
		}

		animation_set->at(ani)->Render(x, y, alpha);
	}
	
	
	//RenderBoundingBox();
}

void CSophia::SetState(int state)
{
	CGameObject::SetState(state);
	
	int ani;
	switch (state)
	{
	case SOPHIA_STATE_WALKING_RIGHT:
		vx = SOPHIA_WALKING_SPEED;
		nx = 1;
		
		break;
	case SOPHIA_STATE_WALKING_LEFT:
		vx = -SOPHIA_WALKING_SPEED;
		nx = -1;
		break;
	case SOPHIA_STATE_JUMP:
		isJump = true;
		vy -= SOPHIA_JUMP_SPEED_Y;
		break;
	case SOPHIA_STATE_IDLE:
		vx = 0;
		
		break;


	}
	//DebugOut(L"\nS: ", state);
}

void CSophia::Reset()
{
	SetState(SOPHIA_STATE_IDLE);
	SetLevel(1);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CSophia::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;

	right = x + SOPHIA_JASON_BBOX_WIDTH;
	bottom = y + SOPHIA_JASON_BBOX_HEIGHT;
	
}
