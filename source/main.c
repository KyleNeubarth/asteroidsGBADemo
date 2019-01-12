#include <string.h>

#include "tonc.h"

#include "spaceship.h"

//get obj buffer ready
OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;

int main() {
	memcpy(&tile_mem[4][0],spaceshipTiles,spaceshipTilesLen);
	memcpy(pal_obj_mem,spaceshipPal,spaceshipPalLen);

	oam_init(obj_buffer, 128);

	//obj_set_attr(obj_buffer,ATTR0_SQUARE,ATTR1_SIZE_32x32,ATTR2_PALBANK(0));
	obj_set_attr(obj_buffer,ATTR0_SQUARE | ATTR0_AFF_DBL,ATTR1_SIZE_32x32 | ATTR1_AFF_ID(0),ATTR2_PALBANK(0) );

	REG_DISPCNT= DCNT_MODE1 | DCNT_OBJ | DCNT_OBJ_1D;

	obj_set_pos(obj_buffer,SCREEN_WIDTH/2-16,SCREEN_WIDTH/2-16);
	
	float x = SCREEN_WIDTH/2-16;
	float y = SCREEN_HEIGHT/2-16;

	u16 rot = 0;
	int scale = 300;

	while(true) {
		vid_vsync();
		key_poll();

		rot += key_tri_horz();
		scale += key_tri_vert();
		x += lu_sin(1000*rot)/(float)6000;
		y += lu_cos(1000*rot)/(float)6000;
		obj_aff_rotscale(obj_aff_buffer,scale,scale,rot*1000);
		obj_set_pos(obj_buffer,x,y);
		oam_copy(oam_mem, obj_buffer, 4);
	}
}