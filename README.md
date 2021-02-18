# pianotiles_autoclicker

pianotiles_autoclicker is a script that can play piano tiles 2. 

It takes a screenshot and scans for pixels on the screen corresponding to the colour of the tile. It then clicks below where the pixel was detected to account for the distance the tile moves during the script's reaction time. The compensation distance scales with time as the game accelerates.

## Usage

I have not added autoamatic adaptibility for different positions of the game window, so users will have to set the target pixels manually.

Additionally, each level will require different presets for initial speed and acceleration:

```bash
beginner challlenge - speedgradient 0.037, initialspeed 3.33
single tile challenge - speedgradient 0.0875, intialspeed 5
```  

To use, simply complie and run the autoclicker.cpp file. Press s to start, and q to quit the program. 

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)
