# 4 Degree of Freedom Arm!
## Introduction
I began this project to create a portfolio in hardware, as previously I only had software experience.
I bought a 3D Printer, a couple servos, and used some parts from an old Arduino kit.
Around this time, I heard about stardance at FRC Worlds. So I decided to create this project in Stardance.
This was basically my Summer Project, and there is still much to expand. I hope you enjoy this!
I think in the future I will add a camera to detect and pick up objects and hopefully do some cool automation things, but unfortunately stardance is ending, but I do have some models trained already so I can hopefully implement them.

This readme will go in stages, or joints, from my turntable to my wrist.
All code will be at the end, with schematics, as this is mainly a project about my Hardware journey.

My names in code are in stages and so this readme should serve as a good key for it to make more sense.

CAD LINK: https://cad.onshape.com/documents/c7c8b1d49425d6c15c1b6fcf/w/1f61bd923d6c5709c7aad9cb/e/c750b924d66f9100911cf1d1?renderMode=0&uiState=6ae6a63f0fa04caa7f0796c0
Note: Onshape is super slow if it is your first time opening a document. If it takes forever, that is expected, do not worry.
It also requires a decent PC to render, iGPU works fine I have a radeon 860m.

### What did I want the arm to originally have?
I wanted a turntable, shoulder, elbow, wrist, and claw

### What did I end up making?
I made a turntable, shoulder, elbow, wrist pitch, wrist roll, and claw.

## Turntable
The turntable was quite easy to begin with. I started with a 28BYJ stepper motor to drive it, originally at a 4:5 Ratio.
Unfortunately, this sucked. Once I put more weight on the arm, it crumbled and started skipping, it was fast, but not strong.
Afterwards, I switched to a 1:8 ratio, which is the limit of my model right now because the pinion gear is basically the size of my drive shaft.
This proved to work very well, and I have seldom had issues with it.

<img width="653" height="332" alt="image" src="https://github.com/user-attachments/assets/b06207a1-0a3f-488b-9e8e-6d4200eb9916" />

There are 2 holes in the center to mount the next stage. 

## Stage #1: Shoulder
This was honestly the easiest one to do, I knew that I needed a good deal of power so I went with an MG996R

<img width="206" height="235" alt="image" src="https://github.com/user-attachments/assets/4e4a81ea-8fe8-49eb-bd49-438f91709b3a" />

It mounts onto the turntable with the 2 spokes at the bottom which match the holes in the turntable

## Stage #2: Elbow
Now this was a bit weirder, I had to design 2 versions in order for it to work.

I had originally intended to just use a MG90s, but it turned out that was farrr too weak for this project, cad below.

<img width="182" height="200" alt="image" src="https://github.com/user-attachments/assets/b11b2bf2-1f30-480a-b923-7cb1b6ae3412" />

In order to fix that, I switched to the much more powerful MG996R

<img width="177" height="170" alt="image" src="https://github.com/user-attachments/assets/aad10211-1538-40fc-b2cc-5a47d55597d2" />

## Stage #3: Wrist (Pitch)

<img width="191" height="176" alt="image" src="https://github.com/user-attachments/assets/abf2a005-9f57-4824-a663-5b9101364b41" />

Quite simple, a MG90s screws onto the end of Stage 2, with a servo arm. This controls the pitch of my claw stage.

## Stage #3 Wrist (Roll)
This is a recent addition that added another degree of freedom to my Arm. 

<img width="318" height="101" alt="image" src="https://github.com/user-attachments/assets/a94c21d9-809c-4b2c-8982-4ac408071d6b" />

Simply goes on top of the pitch servo to rotate the claw

## The Claw
The claw has gone through 2 major designs.

### V1
Originally, I had a radial claw design, something that you normally see when you think of a claw (think of your thumb and your index finger pinching something)
This design sucked, because I am using a robotic arm and not a person who can think, a lot of time it was hard to preciesely pick something up as the height values would change every time I would try to pick something up.

<img width="326" height="282" alt="Screenshot 2026-10-31 203510" src="https://github.com/user-attachments/assets/86f4e83b-6163-4550-84cc-51dcd9720537" />

Yes, it worked, but it needed a lot of improvement

### V2
I opted for a rack and pinion method for my new claw. 

<img width="419" height="368" alt="image" src="https://github.com/user-attachments/assets/f4892de7-16fd-4e3c-a62a-704f0ed936d6" />

This used linear motion instead of rotational, which meant I could pick things up more precisely.
Although not pictured, but I cut up a portion of a sponge and glued it on so that it could conform to whatever I was picking up.

<img width="419" height="368" alt="image" src="https://github.com/user-attachments/assets/a3f40595-465e-4c9f-b4d5-929688f7a73b" />

Thats the CAD over!!

## Wiring

Wiring was pretty simple, not much to remark, schematic below. One pin per servo

<img width="547" height="346" alt="image" src="https://github.com/user-attachments/assets/e762bc09-9c23-4e40-8014-0a7e46cbd6e0" />

I love KiCad Labels so much

## Code
All my code was done through Arduino IDE on an esp32s3
AI was used in order for some kinematics stuff but it was mostly self-coded (C++ is no joke man)
Inn order to fix the power requirements (i dont have enough power rn) my servos run intermittently, only stepping 10 degrees at a time. I find this saves a lot of power

## BOM 
Bom is included in readme

## IRL Photos
<img width="5712" height="4284" alt="IMG_8700" src="https://github.com/user-attachments/assets/678be8bb-4977-4ec6-8ef7-ce791e89159a" />
<img width="4284" height="5712" alt="IMG_8699" src="https://github.com/user-attachments/assets/58d17680-71fb-4ed4-abde-4c4d916d59c2" />


