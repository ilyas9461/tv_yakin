
## **Fuzzy Logic Approach to the Problem of Young Children Watching Television Closely**

### Introduction :

Nowadays, there is almost no home without a television. Many parents have closely witnessed their children watching television during their upbringing and have tried to prevent it in their own ways. There is probably no parent who would not be disturbed by the sight of a child watching his favorite cartoon or commercial in front of the television.

Some studies on why children watch television closely indicate that this is a result, not a cause and that the child may have one of the eye defects. In addition, the child's attention, the vividness of the stimuli, etc. For reasons such as It is thought that he watches television closely ("What should be the distance to watch television?").

It has been determined that watching television closely does not cause any great harm to children and especially to their eyes in the early stages. Eye fatigue, looking at shiny surfaces, exposure to even a small electromagnetic field, etc. It has been determined that it may cause harm (“Closely Harms of Watching Television", 2012)("What are the Harms of Technology to Children") ("Harms of Watching Television Closely"). Its most important effect is that it instils the habit of watching television, and its harmful effects on young children in terms of development cannot be counted.

Despite everything, it is unthinkable that any parent would consent to their children watching television closely. Especially today, when experts recommend that children between the ages of 0-4 should not watch television at all, if possible.

However, parents are left alone in this regard in the chaos of social life. The allure of television and the intensity of social life have left them in a helpless situation. There is almost no parent who does not experience their children watching television closely.
Effective television viewing distance comes to mind here. Studies have concluded that a distance of twice the screen size of the television (corner to corner) may be appropriate. Effective
And for a healthy viewing, it is necessary to be outside this distance. As you get closer, the probability of the problems described above increases ("LED TV Viewing Distances").

So, what are the solution suggestions to eliminate this type of behaviour in the child? In general, most of the solutions given consist of recommendations ("What should be the distance to watch television?"). In our research, we could not find any features of the devices, vehicles or televisions used.

### **The goal of the project** :
To develop a system that prevents young children from watching television closely and encourages children to avoid watching television closely.

### **Method** :
Our first studies focused on a structure that could operate by measuring the distance with a sensor that can measure distance and turning off the television when entering the detected limit and turning on the television when leaving.

![method](foto/ilk_calisma.png)

However, this approach works according to strict logical rules and cannot fully predict the behaviour of a small child in front of the television or the behaviour of any person in front of the television. The way it worked was not suitable for use. We saw that this control structure was incomplete.

At this point, we focused on a method that could analyze the behaviour in front of the television and detect the phenomenon of close viewing by adapting to the conditions in which the television is located.
The solution algorithm of the device to be developed must be compatible with human thought and life. We saw that fuzzy logic approaches are suitable for this (Elmas, 2007) and we focused on developing solutions with fuzzy logic in our study.

**Sharp Logic and Fuzzy Logic**

Traditional logic treats events as 0 or 1 (present or absent, true or false). There are only two situations for an event. Fuzzy logic expresses events with their real values ​​between 0 and 1 (“Fuzzy Logic”)

**Development of an Application that Prevents Young Children from Watching Television Closely**

In order to analyze the movements of a child in front of the television, it would be appropriate to take into account the distance and the time spent at that distance. In other words, the amount of movement and inactivity will need to be examined to understand that television is being watched. In this case, our system will actually reach the result by performing a motion-inactivity analysis at the limit distance determined for the television.
As a method of turning the television on and off, when the on/off button is pressed on the remote control, the information sent to the infrared receiver of the television is pre-captured and recorded by the system.
It was concluded that sending it again in accordance with the fuzzy inference result would be the most appropriate option for use.

Accordingly, the list of materials to be used in the hardware is as follows;
- Arduino Uno or Mega.
- IR receiver and transmitter
- HC-RS04 ultrasonik sensor (2 pieces).
- Led (2 pieces).


![blok picture](foto/genelBlokSema.jpg)



**Used technologies :**

1. Microcontrollers Arduino UNO (Atmega328P)
2. Fuzzy Logic and c++
3. Arduino programming with C++.
4. Sense distance technics.
5. Eclstronic circuits and PCB 

## **Note** : 
This work ,

It ranked **third in TURKEY** in the COMPUTER branch of the TUBITAK 47th SECONDARY EDUCATION STUDENTS RESEARCH PROJECTS COMPETITION (06.05.2016).
<a href="https://tubitak.gov.tr/tr/yarismalar/2204-lise-ogrencileri-arastirma-projeleri-yarismasi" target="_blank">
	(TUBITAK High School Students Research Projects Competition)
</a>
