# FieldTest
Includes a series of different starter tests we would run in the field to help debug issues quickly

## Running Executables
Remember, it is important to build in the main folder (see primary README). To run an executable, do the following
```
cd /path/to/FullscalePrime/build/FieldTest
sudo ./<EXECUTABLE_NAME>
```

## File Explanations
liftServoTest: Runs lift servo to either lock or unlock the lift

levelServoTest: Runs level servo to tilt the level up or down

levelLockTest: Locks the level at its default configuration

nacelleServoTest: Runs nacelle locking servo to either lock or unlock the nacelles

liftArrowTest: Runs lift up or down

rcbArrowTest: Runs RCB Motor clockwise or counterclockwise

swivel1Test: Swivels nacelle 1 left or right

swivel2Test: Swivels nacelle 2 left or right

swivel3Test: Swivels nacelle 3 left or right
