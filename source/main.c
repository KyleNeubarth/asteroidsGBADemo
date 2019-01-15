#include <string.h>

#include "tonc.h"

#include "spaceship.h"

//get obj buffer ready
OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;

int numBullets = 0;
float bulletCoords[10][2];

void spawnBullet(float x, float y) {
	numBullets++;
	obj_set_attr(&obj_buffer[numBullets],ATTR0_SQUARE | ATTR0_AFF,ATTR1_SIZE_8x8 | ATTR1_AFF_ID(numBullets),ATTR2_PALBANK(0) );
	obj_set_pos(&obj_buffer[numBullets],x,y);

}

int main() {
	memcpy(&tile_mem[4][0],spaceshipTiles,spaceshipTilesLen);
	memcpy(pal_obj_mem,spaceshipPal,spaceshipPalLen);

	oam_init(obj_buffer, 128);

	obj_set_attr(obj_buffer,ATTR0_SQUARE | ATTR0_AFF_DBL,ATTR1_SIZE_32x32 | ATTR1_AFF_ID(0),ATTR2_PALBANK(0) );

	OBJ_ATTR* bullet = &obj_buffer[1];
	//aff id is still 0 because it will share the orientation of the ship
	REG_DISPCNT= DCNT_MODE1 | DCNT_OBJ | DCNT_OBJ_1D;

	obj_set_pos(obj_buffer,SCREEN_WIDTH/2-16,SCREEN_WIDTH/2-16);

	float x = SCREEN_WIDTH/2-32;
	float y = SCREEN_HEIGHT/2-32;

	u16 rot = 0;
	int scale = 320;

	float speed = 0;
	float friction = .99f;

	while(true) {
		vid_vsync();
		key_poll();

		rot += key_tri_horz();
		scale += key_tri_vert();
		if (key_held(1<<KI_A)) {
			speed += .04f;
		}
		speed *= friction;
		//holy shit this is inefficient
		int sin = lu_sin(500*rot);
		int cos = lu_cos(500*rot);
		x += speed*sin/(float)6000;
		y += speed*cos/(float)6000;

		if (key_hit(1<<KI_B) && numBullets < 10) {
			float bx = x + 32 + 50*sin/(float)6000;
			float by = y + 32 + 50*cos/(float)6000;
			bulletCoords[numBullets][0] = bx;
			bulletCoords[numBullets][1] = by;
			spawnBullet(bx,by);
			obj_aff_rotscale(&obj_aff_buffer[numBullets],scale,scale,rot*500);
		}
		for (int i=1;i<=numBullets;i++) {
			float bx = bulletCoords[i-1][0];
			float by = bulletCoords[i-1][1];
			obj_set_pos(&obj_buffer[i],bx+1,by);
			bulletCoords[i-1][0]++;

		}

		obj_aff_rotscale(obj_aff_buffer,scale,scale,rot*500);
		obj_set_pos(obj_buffer,x,y);
		oam_copy(oam_mem, obj_buffer, 128);
	}
}