# ws2801-github-contributions

<div align="center"><img src="https://img.shields.io/static/v1?label=MADE%20BY&message=ITegs&color=blueviolet&style=for-the-badge&logo=github&link=https://github.com/ITegs" /><br/><br/></div>

Show your github condribution calendar on a ws2801 rgb strip.\
\
Automatically fetches the condributions of the last 12 months from the Github graphql API and then displays them on a ws2801 rgb-strip.\
The color of the day depends on the number of condrubutions on that day.\
The rgb-strip is updated every 5 minutes.

## Configuration

### Code:

1. Remove `#include "data.h"`
1. Add your wifi **SSID** and **Password**
1. Add your **GitHub-Token**
1. Change `NUM_LEDS` to the number of leds your rgb-stripe has

### Hardware:

1. Connect the `GND` and `5V` of the rgb-strip to the `GND` and `5V` of the arduino
1. Connect the `Data`-pin of the rgb-strip to pin number **25** on the arduino
1. Connect the `Clock`-pin of the rgb-strip to pin number **26** on the arduino
