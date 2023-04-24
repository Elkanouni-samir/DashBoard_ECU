# DashBoard_ECU
This repository is a part of a big project, this repository hold the code and some illustartions that are going to be ran on a raspberry pico microcontroller to ensure some functions to be discussed later.
</br> your discription sucks

<H1> functionnalities </H>
<ul>
<li> through CAN communication, get the value of RPM </li>
<li> update the value shown on the dashboard to the new RPM </li>
<li> read inputs from switches (windows controls, turn signals, ...) </li>
<li> update the state of the corresponding outputs </li>
<li> + ? </li>
</ul>

<H1> Hardware </H1>

<H2> the controller </H2>
a rasberry pi pico is used for the main control and logic <a href="https://datasheets.raspberrypi.com/pico/pico-datasheet.pdf"> <b> Datasheet here </b> </a>
as the pico doesn't support CAN protocol, we'll be using a CAN to SPI module [i forgot the name add it here] 

<H2> the toolchaine </H2>
the programming is going to be entirely in micropython, we'll be using the Thonny IDE. if better options are available please let me know

