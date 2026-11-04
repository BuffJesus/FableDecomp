"""Fresh VC71 parity gate: CInputEvent setters (Ego_r lib_input_event.cpp order; held events copy movement into the first union, MouseMovement into the second); supersede the CInputEventRetail stand-ins."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
F=R/'rebuild/src/compiled/00/a0/CInputEvent_Setters_00a03bc0.cpp'
ROOTS=[
 dict(name='set_mouse_pos',source=F,symbol='?SetMousePos@CInputEvent@@QAEXABVC2DVector@@@Z',address=0xa03bc0,size=18,instructions=6,relocations=0),
 dict(name='set_as_null',source=F,symbol='?SetAsNull@CInputEvent@@QAEXXZ',address=0xa03be0,size=9,instructions=4,relocations=0),
 dict(name='set_as_keyboard_key_pressed',source=F,symbol='?SetAsKeyboardKeyPressed@CInputEvent@@QAEXW4EInputKey@@N@Z',address=0xa03bf0,size=33,instructions=8,relocations=0),
 dict(name='set_as_keyboard_key_held',source=F,symbol='?SetAsKeyboardKeyHeld@CInputEvent@@QAEXW4EInputKey@@NN@Z',address=0xa03c20,size=34,instructions=10,relocations=0),
 dict(name='set_as_keyboard_key_released',source=F,symbol='?SetAsKeyboardKeyReleased@CInputEvent@@QAEXW4EInputKey@@NN@Z',address=0xa03c50,size=37,instructions=9,relocations=0),
 dict(name='set_as_left_mouse_button_pressed',source=F,symbol='?SetAsLeftMouseButtonPressed@CInputEvent@@QAEXABVC2DVector@@N@Z',address=0xa03c80,size=42,instructions=11,relocations=0),
 dict(name='set_as_char_pressed',source=F,symbol='?SetAsCharPressed@CInputEvent@@QAEXGN@Z',address=0xa03cb0,size=35,instructions=8,relocations=0),
 dict(name='set_as_char_released',source=F,symbol='?SetAsCharReleased@CInputEvent@@QAEXGN@Z',address=0xa03ce0,size=35,instructions=8,relocations=0),
 dict(name='set_as_left_mouse_button_held',source=F,symbol='?SetAsLeftMouseButtonHeld@CInputEvent@@QAEXABVC2DVector@@ABVC3DVector@@NN@Z',address=0xa03d10,size=66,instructions=19,relocations=0),
 dict(name='set_as_left_mouse_button_released',source=F,symbol='?SetAsLeftMouseButtonReleased@CInputEvent@@QAEXABVC2DVector@@NN@Z',address=0xa03d60,size=46,instructions=12,relocations=0),
 dict(name='set_as_middle_mouse_button_pressed',source=F,symbol='?SetAsMiddleMouseButtonPressed@CInputEvent@@QAEXABVC2DVector@@N@Z',address=0xa03d90,size=42,instructions=11,relocations=0),
 dict(name='set_as_middle_mouse_button_held',source=F,symbol='?SetAsMiddleMouseButtonHeld@CInputEvent@@QAEXABVC2DVector@@ABVC3DVector@@NN@Z',address=0xa03dc0,size=66,instructions=19,relocations=0),
 dict(name='set_as_middle_mouse_button_released',source=F,symbol='?SetAsMiddleMouseButtonReleased@CInputEvent@@QAEXABVC2DVector@@NN@Z',address=0xa03e10,size=46,instructions=12,relocations=0),
 dict(name='set_as_right_mouse_button_pressed',source=F,symbol='?SetAsRightMouseButtonPressed@CInputEvent@@QAEXABVC2DVector@@N@Z',address=0xa03e40,size=42,instructions=11,relocations=0),
 dict(name='set_as_right_mouse_button_held',source=F,symbol='?SetAsRightMouseButtonHeld@CInputEvent@@QAEXABVC2DVector@@ABVC3DVector@@NN@Z',address=0xa03e70,size=66,instructions=19,relocations=0),
 dict(name='set_as_right_mouse_button_released',source=F,symbol='?SetAsRightMouseButtonReleased@CInputEvent@@QAEXABVC2DVector@@NN@Z',address=0xa03ec0,size=46,instructions=12,relocations=0),
 dict(name='set_as_extra_mouse_button_pressed',source=F,symbol='?SetAsExtraMouseButtonPressed@CInputEvent@@QAEXABVC2DVector@@NJ@Z',address=0xa03ef0,size=45,instructions=13,relocations=0),
 dict(name='set_as_extra_mouse_button_held',source=F,symbol='?SetAsExtraMouseButtonHeld@CInputEvent@@QAEXABVC2DVector@@ABVC3DVector@@NNJ@Z',address=0xa03f20,size=69,instructions=21,relocations=0),
 dict(name='set_as_extra_mouse_button_released',source=F,symbol='?SetAsExtraMouseButtonReleased@CInputEvent@@QAEXABVC2DVector@@NNJ@Z',address=0xa03f70,size=49,instructions=14,relocations=0),
 dict(name='set_as_mouse_movement',source=F,symbol='?SetAsMouseMovement@CInputEvent@@QAEXABVC2DVector@@ABVC3DVector@@N@Z',address=0xa03fb0,size=67,instructions=21,relocations=0),
 dict(name='set_as_mouse_wheel_movement',source=F,symbol='?SetAsMouseWheelMovement@CInputEvent@@QAEXMN@Z',address=0xa04000,size=34,instructions=8,relocations=0),
 dict(name='set_as_joystick_position',source=F,symbol='?SetAsJoystickPosition@CInputEvent@@QAEXABVC2DVector@@N@Z',address=0xa04030,size=41,instructions=11,relocations=0),
 dict(name='set_as_joystick_position2',source=F,symbol='?SetAsJoystickPosition2@CInputEvent@@QAEXABVC2DVector@@N@Z',address=0xa04060,size=41,instructions=11,relocations=0),
 dict(name='set_as_joystick_button_pressed',source=F,symbol='?SetAsJoystickButtonPressed@CInputEvent@@QAEXJMABVC2DVector@@N@Z',address=0xa04090,size=55,instructions=15,relocations=0),
 dict(name='set_as_joystick_button_held',source=F,symbol='?SetAsJoystickButtonHeld@CInputEvent@@QAEXJMABVC2DVector@@N@Z',address=0xa040d0,size=55,instructions=15,relocations=0),
 dict(name='set_as_joystick_button_released',source=F,symbol='?SetAsJoystickButtonReleased@CInputEvent@@QAEXJMABVC2DVector@@N@Z',address=0xa04110,size=55,instructions=15,relocations=0),
 dict(name='set_joystick_button_pressure',source=F,symbol='?SetJoystickButtonPressure@CInputEvent@@QAEXM@Z',address=0xa04150,size=10,instructions=3,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-input-event-setters.json',[],
  'CInputEvent setters (Ego_r lib_input_event.cpp order; held events copy movement into the first union, MouseMovement into the second); supersede the CInputEventRetail stand-ins')
 print('NATIVE_INPUT_EVENT_SETTERS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
