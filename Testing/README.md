# Testing:
## v0.0
### Hardware Watchdog:
Test functionality of watchdog (TPL5010) timer and confirm function with Particle Boron. 

Tested with Particle Boron connected to Feather socket, board powered via BatIn connection (3.7V power supply). Arduino was connected externally (common ground) and used to measure tripping on reset pin and timing of these pulses. The testing program was [ExternalResetTest](Software/ExternalResetTest/ExternalResetTest.ino). For the first test, the Boron was simply left in an idle state to ensure the watchdog would properly trigger at intervals. For the second test, the watchdog was reset by the Boron (via a high pulse 1ms in durration) every 5 minutes, which should prevent the watchdog from ever triggering. For all tests, a 100k&Omega; set resistor was used for the watchdog timer. 

Calculated period = 2159.65368 s (35.994 min)

T = [(((R<sub>Set</sub> x 2 x A)/100 + B )<sup>2</sup> - B<sup>2</sup>)/(4 x A) + C] / 100

for T > 1000s, A = 0.3177, B = -135.2571, C = 34522.4680 (See [Datasheet](http://www.ti.com/lit/ds/symlink/tpl5010.pdf) for more values/details)

<b>Test 1:</b>

<b>Test 2:</b>

## Alternative Parts:
Alternative clock, v0.4A board

| Previous Manufacturer Part Number | Previous Manufacturer | Alt Manufacturer Part Number | Alt Manufacturer | 
| ------------- | -------- | ---- | ---- |
| ABS07-120-32.768KHZ-T | Abracon | ECS-.327-6-34QN-TR | ECS | 

Alternative crystal oscilation compaired with previous part on the v0.4A board, with MCP79412 RTC

**Standard Part**

![Crystal Part Test](https://github.umn.edu/gemsiot/Project-Hyena/blob/master/Testing/TestingCaptures/CrystalTest.PNG)

Yellow trace is connected to `X1` pin on RTC, blue trace is connected to `X2` pin on RTC, math output is CH1 - CH2

**Alt Part**

![Crystal Alternative Part Test](https://github.umn.edu/gemsiot/Project-Hyena/blob/master/Testing/TestingCaptures/CrystalTest_AltPart.PNG)

Yellow trace is connected to `X1` pin on RTC, blue trace is connected to `X2` pin on RTC, math output is CH1 - CH2



## Issues:
### <b>Inrush current from Soil Moisture Symbiont (v0.0) trips external power switch</b>
<b>Cause:</b>
When the external switch is triggered with the Soil Moisture Symbiont connected, the inrush current on the soil moisture symbiont exceeds the current limit (100mA) on the load switch (MIC2091) for a long enough time to cause the fault detection to trip and disconnect the output. This results in a locked up state, since on each re-try the same overload occurs. 

Here we see the scope capture of the power on of the soil moisture symbiont when powered by a bench supply (current limit > max draw of soil moisture symbiont)
![Power Supply Test](https://github.umn.edu/gemsiot/Project-Hyena/blob/master/Testing/TestingCaptures/TEK0000.JPG)

Yellow -> V<sub>Supply</sub>

Blue -> I<sub>Supply</sub> (10V/A)


<b>Potential Solutions:</b>

- Change boost converter on Soil Moisture Symbiont to a synchronous converter to prevent this inrush current. Fixes current problem, but not global potential issue. 
- Change load switch to one with a higher current limit, this would allow the current inrush current spike to be present without tripping the device into a locked out state.

<b>Testing:</b>

Switched the load switch to the MIC2005A load switch (identical pinout), which has a 500mA current limit, but otherwise the same functionality as the previous switch. The test of the system with this switch are seen below

![MIC2005A Test](https://github.umn.edu/gemsiot/Project-Hyena/blob/master/Testing/TestingCaptures/TEK0004.JPG)

Yellow -> V<sub>Supply</sub>

Blue -> I<sub>Supply</sub> (10V/A)


As you can see, we get the same high value spike, but the switch does not trigger an overload and instead properly switches the output on.