#pragma bank 255

#include "camera.h"

#include "actor.h"
#include "data_manager.h"
#include "math.h"

#define CAMERA_FIXED_OFFSET_X PX_TO_SUBPX(8)
#define CAMERA_FIXED_OFFSET_Y PX_TO_SUBPX(8)

static UBYTE camera_offset_step(UBYTE speed) {
    if (speed == 0) {
        return 0; // instant
    }
    if (speed > 8) {
        return 1; // fractional speeds: one pixel when they tick
    }
    return speed;
}

// Keep look-ahead from asking for more offset than the map can scroll.
// Pull back gradually so arriving at / leaving an edge stays smooth.
static void camera_limit_offset_to_map_room(void) {
    UWORD player_px = SUBPX_TO_PX(PLAYER.pos.x);
    UWORD player_py = SUBPX_TO_PX(PLAYER.pos.y);
    WORD max_pos_x = 0;
    WORD min_neg_x = 0;
    WORD max_pos_y = 0;
    WORD min_neg_y = 0;
    UBYTE step_x = camera_offset_step(lookahead_speed_x);
    UBYTE step_y = camera_offset_step(lookahead_speed_y);
    UWORD room;

    // Camera center ≈ player - offset. Scroll needs center in
    // [SCREEN_WIDTH_HALF, image_width - SCREEN_WIDTH_HALF].
    if (player_px > SCREEN_WIDTH_HALF) {
        room = player_px - SCREEN_WIDTH_HALF;
        max_pos_x = (room > 127) ? 127 : (WORD)room;
    }
    if (image_width > SCREEN_WIDTH_HALF && player_px < (UWORD)(image_width - SCREEN_WIDTH_HALF)) {
        room = (UWORD)(image_width - SCREEN_WIDTH_HALF) - player_px;
        min_neg_x = (room > 128) ? -128 : -(WORD)room;
    }

    if (player_py > SCREEN_HEIGHT_HALF) {
        room = player_py - SCREEN_HEIGHT_HALF;
        max_pos_y = (room > 127) ? 127 : (WORD)room;
    }
    if (image_height > SCREEN_HEIGHT_HALF && player_py < (UWORD)(image_height - SCREEN_HEIGHT_HALF)) {
        room = (UWORD)(image_height - SCREEN_HEIGHT_HALF) - player_py;
        min_neg_y = (room > 128) ? -128 : -(WORD)room;
    }

    if (camera_offset_x > max_pos_x) {
        if (step_x == 0 || (WORD)(camera_offset_x - (BYTE)step_x) <= max_pos_x) {
            camera_offset_x = (BYTE)max_pos_x;
        } else {
            camera_offset_x -= (BYTE)step_x;
        }
    } else if (camera_offset_x < min_neg_x) {
        if (step_x == 0 || (WORD)(camera_offset_x + (BYTE)step_x) >= min_neg_x) {
            camera_offset_x = (BYTE)min_neg_x;
        } else {
            camera_offset_x += (BYTE)step_x;
        }
    }

    if (camera_offset_y > max_pos_y) {
        if (step_y == 0 || (WORD)(camera_offset_y - (BYTE)step_y) <= max_pos_y) {
            camera_offset_y = (BYTE)max_pos_y;
        } else {
            camera_offset_y -= (BYTE)step_y;
        }
    } else if (camera_offset_y < min_neg_y) {
        if (step_y == 0 || (WORD)(camera_offset_y + (BYTE)step_y) >= min_neg_y) {
            camera_offset_y = (BYTE)min_neg_y;
        } else {
            camera_offset_y += (BYTE)step_y;
        }
    }
}

UINT16 camera_x;
UINT16 camera_y;
UINT16 camera_clamp_x;
UINT16 camera_clamp_y;
BYTE camera_offset_x;
BYTE camera_offset_y;
BYTE camera_deadzone_x;
BYTE camera_deadzone_y;
UBYTE camera_settings;

// CAMERAPLUS DEFAULT VARIABLES
UBYTE camplus_status;
UBYTE camplus_init;
BYTE camplus_offset_x;
BYTE camplus_offset_y;
BYTE camplus_deadzone_x;
BYTE camplus_deadzone_y;

// CAMERAPLUS LOOK-AHEAD VARIABLES
BYTE lookahead_offset_x;
BYTE lookahead_offset_y;
UBYTE lookahead_check_x;
UBYTE lookahead_check_y;
UBYTE lookahead_check_f;
UBYTE lookahead_log_x;
UBYTE lookahead_log_y;
UBYTE lookahead_log_f;
UBYTE lookahead_speed_x;
UBYTE lookahead_speed_y;
UBYTE lookahead_speed_f;
UBYTE lookahead_state;
UBYTE lookahead_behavior;
UBYTE lookahead_init;

void camera_init(void) BANKED {
    camera_settings = CAMERA_LOCK_FLAG;
    camera_x = camera_y = 0;
    camera_offset_x = camera_offset_y = 0;
    camera_clamp_x = camera_clamp_y = 0;
    camera_reset();

    if (camplus_status == 0) {
        lookahead_log_x = 1;
        lookahead_log_y = 1;
        lookahead_log_f = 1;
        if (camplus_init != 1) {
            if ((lookahead_init == 0) && (lookahead_behavior != 3)) {
                if (lookahead_behavior == 0 || lookahead_behavior == 2) {
                    if (PLAYER.dir == DIR_RIGHT) {
                        camera_offset_x = -lookahead_offset_x;
                    } else if (PLAYER.dir == DIR_LEFT) {
                        camera_offset_x = lookahead_offset_x;
                    } else {
                        camera_offset_x = camplus_offset_x;
                    }
                } else {
                    camera_offset_x = camplus_offset_x;
                }
                if (lookahead_behavior == 1 || lookahead_behavior == 2) {
                    if (PLAYER.dir == DIR_DOWN) {
                        camera_offset_y = -lookahead_offset_y;
                    } else if (PLAYER.dir == DIR_UP) {
                        camera_offset_y = lookahead_offset_y;
                    } else {
                        camera_offset_y = camplus_offset_y;
                    }
                } else {
                    camera_offset_y = camplus_offset_y;
                }
            } else {
                camera_offset_x = camplus_offset_x;
                camera_offset_y = camplus_offset_y;
            }
            camera_deadzone_x = camplus_deadzone_x;
            camera_deadzone_y = camplus_deadzone_y;
        }
    }
}

void camera_update(void) BANKED {
    if (camplus_status == 0 && lookahead_behavior != 3) {
        camera_limit_offset_to_map_room();
    }

    if (camera_settings & CAMERA_LOCK_X_FLAG) {
        // Signed offset in subpixels. Do not use PX_TO_SUBPX(BYTE) — left-shift of a
        // negative BYTE promotes badly and wraps the camera target.
        INT16 offset_subpx = ((INT16)camera_offset_x) << 5;
        UWORD player_cam = PLAYER.pos.x + CAMERA_FIXED_OFFSET_X;
        UWORD target_pos;
        WORD tolerance = PX_TO_SUBPX(camera_deadzone_x);
        UWORD target_min_pos;
        UWORD target_max_pos;
        UWORD new_cam_pos = camera_x;

        if (offset_subpx >= 0) {
            UWORD off = (UWORD)offset_subpx;
            target_pos = (player_cam > off) ? (UWORD)(player_cam - off) : 0;
        } else {
            UWORD off = (UWORD)(-offset_subpx);
            if (player_cam > (UWORD)(0xFFFF - off)) {
                target_pos = 0xFFFF;
            } else {
                target_pos = (UWORD)(player_cam + off);
            }
        }

        target_min_pos = ((UWORD)tolerance > target_pos) ? 0 : (UWORD)(target_pos - (UWORD)tolerance);
        if (target_pos > (UWORD)(0xFFFF - (UWORD)tolerance)) {
            target_max_pos = 0xFFFF;
        } else {
            target_max_pos = (UWORD)(target_pos + (UWORD)tolerance);
        }

        if (new_cam_pos < target_min_pos) {
            new_cam_pos = target_min_pos;
            if ((camera_settings & CAMERA_LOCK_X_MAX_FLAG) && new_cam_pos > camera_clamp_x) {
                new_cam_pos = camera_clamp_x;
            }
        } else if (new_cam_pos > target_max_pos) {
            new_cam_pos = target_max_pos;
            if ((camera_settings & CAMERA_LOCK_X_MIN_FLAG) && new_cam_pos < camera_clamp_x) {
                new_cam_pos = camera_clamp_x;
            }
        }
        camera_x = camera_clamp_x = new_cam_pos;
    }

    if (camera_settings & CAMERA_LOCK_Y_FLAG) {
        INT16 offset_subpx = ((INT16)camera_offset_y) << 5;
        UWORD player_cam = PLAYER.pos.y + CAMERA_FIXED_OFFSET_Y;
        UWORD target_pos;
        WORD tolerance = PX_TO_SUBPX(camera_deadzone_y);
        UWORD target_min_pos;
        UWORD target_max_pos;
        UWORD new_cam_pos = camera_y;

        if (offset_subpx >= 0) {
            UWORD off = (UWORD)offset_subpx;
            target_pos = (player_cam > off) ? (UWORD)(player_cam - off) : 0;
        } else {
            UWORD off = (UWORD)(-offset_subpx);
            if (player_cam > (UWORD)(0xFFFF - off)) {
                target_pos = 0xFFFF;
            } else {
                target_pos = (UWORD)(player_cam + off);
            }
        }

        target_min_pos = ((UWORD)tolerance > target_pos) ? 0 : (UWORD)(target_pos - (UWORD)tolerance);
        if (target_pos > (UWORD)(0xFFFF - (UWORD)tolerance)) {
            target_max_pos = 0xFFFF;
        } else {
            target_max_pos = (UWORD)(target_pos + (UWORD)tolerance);
        }

        if (new_cam_pos < target_min_pos) {
            new_cam_pos = target_min_pos;
            if ((camera_settings & CAMERA_LOCK_Y_MAX_FLAG) && new_cam_pos > camera_clamp_y) {
                new_cam_pos = camera_clamp_y;
            }
        } else if (new_cam_pos > target_max_pos) {
            new_cam_pos = target_max_pos;
            if ((camera_settings & CAMERA_LOCK_Y_MIN_FLAG) && new_cam_pos < camera_clamp_y) {
                new_cam_pos = camera_clamp_y;
            }
        }
        camera_y = camera_clamp_y = new_cam_pos;
    }

    if ((lookahead_behavior != 3) && (camplus_status == 0)) {
        if (lookahead_speed_x > 8) {
            switch (lookahead_speed_x) {
                case 16:
                    lookahead_check_x = 2;
                    break;
                case 32:
                    lookahead_check_x = 3;
                    break;
                case 64:
                    lookahead_check_x = 4;
                    break;
                default:
                    break;
            }
        }
        if (lookahead_speed_y > 8) {
            switch (lookahead_speed_y) {
                case 16:
                    lookahead_check_y = 2;
                    break;
                case 32:
                    lookahead_check_y = 3;
                    break;
                case 64:
                    lookahead_check_y = 4;
                    break;
                default:
                    break;
            }
        }
        if (lookahead_speed_f > 8) {
            switch (lookahead_speed_f) {
                case 16:
                    lookahead_check_f = 2;
                    break;
                case 32:
                    lookahead_check_f = 3;
                    break;
                case 64:
                    lookahead_check_f = 4;
                    break;
                default:
                    break;
            }
        }

        if (lookahead_state != 2) {
            if ((lookahead_behavior == 0 || lookahead_behavior == 2)) {
                if (lookahead_state == 1) {
                    if ((camera_offset_x == camplus_offset_x && lookahead_behavior == 0) ||
                        (camera_offset_x == camplus_offset_x && camera_offset_y == camplus_offset_y &&
                         lookahead_behavior == 2)) {
                        lookahead_state = 2;
                    } else if (lookahead_speed_f == 0) {
                        camera_offset_x = camplus_offset_x;
                    } else if (lookahead_speed_f > 8) {
                        if (lookahead_log_f % lookahead_check_f == 0) {
                            lookahead_log_f = 1;
                            if (camera_offset_x > camplus_offset_x) {
                                camera_offset_x = MAX(camplus_offset_x, camera_offset_x - 1);
                            } else if (camera_offset_x < camplus_offset_x) {
                                camera_offset_x = MIN(camplus_offset_x, camera_offset_x + 1);
                            }
                        } else {
                            lookahead_log_f++;
                        }
                    } else if (camera_offset_x > camplus_offset_x) {
                        camera_offset_x = MAX(camplus_offset_x, camera_offset_x - lookahead_speed_f);
                    } else if (camera_offset_x < camplus_offset_x) {
                        camera_offset_x = MIN(camplus_offset_x, camera_offset_x + lookahead_speed_f);
                    }
                } else if (PLAYER.dir == DIR_RIGHT) {
                    if (lookahead_speed_x == 0) {
                        camera_offset_x = -lookahead_offset_x + camplus_offset_x;
                    } else if (lookahead_speed_x > 8) {
                        if (lookahead_log_x % lookahead_check_x == 0) {
                            lookahead_log_x = 1;
                            camera_offset_x =
                                MAX((-lookahead_offset_x + camplus_offset_x), (camera_offset_x - 1));
                        } else {
                            lookahead_log_x++;
                        }
                    } else {
                        camera_offset_x = MAX((-lookahead_offset_x + camplus_offset_x),
                                              (camera_offset_x - lookahead_speed_x));
                    }
                } else if (PLAYER.dir == DIR_LEFT) {
                    if (lookahead_speed_x == 0) {
                        camera_offset_x = lookahead_offset_x + camplus_offset_x;
                    } else if (lookahead_speed_x > 8) {
                        if (lookahead_log_x % lookahead_check_x == 0) {
                            lookahead_log_x = 1;
                            camera_offset_x =
                                MIN((lookahead_offset_x + camplus_offset_x), (camera_offset_x + 1));
                        } else {
                            lookahead_log_x++;
                        }
                    } else {
                        camera_offset_x = MIN((lookahead_offset_x + camplus_offset_x),
                                              (camera_offset_x + lookahead_speed_x));
                    }
                }
            }

            if ((lookahead_behavior == 1 || lookahead_behavior == 2)) {
                if (lookahead_state == 1) {
                    if ((camera_offset_y == camplus_offset_y && lookahead_behavior == 1) ||
                        (camera_offset_x == camplus_offset_x && camera_offset_y == camplus_offset_y &&
                         lookahead_behavior == 2)) {
                        lookahead_state = 2;
                    } else if (lookahead_speed_f == 0) {
                        camera_offset_y = camplus_offset_y;
                    } else if (lookahead_speed_f > 8) {
                        if ((lookahead_log_f + 1) % lookahead_check_f == 0) {
                            lookahead_log_f = 1;
                            if (camera_offset_y > camplus_offset_y) {
                                camera_offset_y = MAX(camplus_offset_y, (camera_offset_y - 1));
                            } else if (camera_offset_y < camplus_offset_y) {
                                camera_offset_y = MIN(camplus_offset_y, (camera_offset_y + 1));
                            }
                        } else {
                            lookahead_log_f++;
                        }
                    } else if (camera_offset_y > camplus_offset_y) {
                        camera_offset_y = MAX(camplus_offset_y, (camera_offset_y - lookahead_speed_f));
                    } else if (camera_offset_y < camplus_offset_y) {
                        camera_offset_y = MIN(camplus_offset_y, (camera_offset_y + lookahead_speed_f));
                    }
                } else if (PLAYER.dir == DIR_DOWN) {
                    if (lookahead_speed_y == 0) {
                        camera_offset_y = -lookahead_offset_y + camplus_offset_y;
                    } else if (lookahead_speed_y > 8) {
                        if (lookahead_log_y % lookahead_check_y == 0) {
                            lookahead_log_y = 1;
                            camera_offset_y =
                                MAX((-lookahead_offset_y + camplus_offset_y), (camera_offset_y - 1));
                        } else {
                            lookahead_log_y++;
                        }
                    } else {
                        camera_offset_y = MAX((-lookahead_offset_y + camplus_offset_y),
                                              (camera_offset_y - lookahead_speed_y));
                    }
                } else if (PLAYER.dir == DIR_UP) {
                    if (lookahead_speed_y == 0) {
                        camera_offset_y = lookahead_offset_y + camplus_offset_y;
                    } else if (lookahead_speed_y > 8) {
                        if (lookahead_log_y % lookahead_check_y == 0) {
                            lookahead_log_y = 1;
                            camera_offset_y =
                                MIN(lookahead_offset_y + camplus_offset_y, (camera_offset_y + 1));
                        } else {
                            lookahead_log_y++;
                        }
                    } else {
                        camera_offset_y = MIN(lookahead_offset_y + camplus_offset_y,
                                              (camera_offset_y + lookahead_speed_y));
                    }
                }
            }

            if (lookahead_state == 0) {
                if ((PLAYER.dir == DIR_DOWN || PLAYER.dir == DIR_UP) &&
                    (camera_offset_x != camplus_offset_x)) {
                    if (lookahead_speed_x == 0) {
                        camera_offset_x = camplus_offset_x;
                    } else if (lookahead_speed_x > 8) {
                        if (lookahead_log_x % lookahead_check_x == 0) {
                            lookahead_log_x = 1;
                            if (camera_offset_x > camplus_offset_x) {
                                camera_offset_x = MAX(camplus_offset_x, camera_offset_x - 1);
                            } else {
                                camera_offset_x = MIN(camplus_offset_x, camera_offset_x + 1);
                            }
                        } else {
                            lookahead_log_x++;
                        }
                    } else if (camera_offset_x > camplus_offset_x) {
                        camera_offset_x = MAX(camplus_offset_x, camera_offset_x - lookahead_speed_x);
                    } else {
                        camera_offset_x = MIN(camplus_offset_x, camera_offset_x + lookahead_speed_x);
                    }
                } else if ((PLAYER.dir == DIR_LEFT || PLAYER.dir == DIR_RIGHT) &&
                           (camera_offset_y != camplus_offset_y)) {
                    if (lookahead_speed_y == 0) {
                        camera_offset_y = camplus_offset_y;
                    } else if (lookahead_speed_y > 8) {
                        if (lookahead_log_y % lookahead_check_y == 0) {
                            lookahead_log_y = 1;
                            if (camera_offset_y > camplus_offset_y) {
                                camera_offset_y = MAX(camplus_offset_y, camera_offset_y - 1);
                            } else {
                                camera_offset_y = MIN(camplus_offset_y, camera_offset_y + 1);
                            }
                        } else {
                            lookahead_log_y++;
                        }
                    } else if (camera_offset_y > camplus_offset_y) {
                        camera_offset_y = MAX(camplus_offset_y, camera_offset_y - lookahead_speed_y);
                    } else {
                        camera_offset_y = MIN(camplus_offset_y, camera_offset_y + lookahead_speed_y);
                    }
                }
            }
        }

        camera_limit_offset_to_map_room();
    }
}
