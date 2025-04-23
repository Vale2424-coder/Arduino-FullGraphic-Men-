### Problem
I'm working on a Marlin-based Arduino firmware that compiles and runs perfectly with the **RepRapDiscount Smart Controller** (text display), but I want to use a **RepRapDiscount Full Graphic Smart Controller** (128x64 ST7920).

As soon as I switch the controller type in `Configuration.h`, the project fails to compile.

### Errors I'm getting:
- `lcd_implementation_drawmenu_setting_edit_float22` not declared
- `fanSpeed` not declared
- `WINDER_PIN` and `EXTRUDER_MOTOR_ON_OFF_PIN` not declared

### Goal
I want the menu and functions to stay **exactly the same as with the basic LCD**, just with graphical output instead of text.

The project files are here:  
👉 [insert your GitHub repo link here]

Any help or suggestions would be **greatly appreciated**! 🙏

Thanks in advance!
