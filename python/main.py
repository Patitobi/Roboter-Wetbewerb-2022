import time
import RPI.GPIO as GPIO

class IRsender():
    def __init__(self):
        pass

class IRempfaenger():
    def __init__(self):
        pass
    
class Reifen():
    def __init__(self, pin=int()):
        self.steuerpin=pin
        self.mode=False
        
        self.setmode(False)
        
    def getMode(self):
        return self.mode
    
    def setmode(self, mode=bool()):
        if mode== True:
            pass
        elif mode==False:
            pass
        
        self.mode=mode

class ULTRAschall():
    def __init__(self, trigger=int(), echo=int()):
        self.trigger=trigger #pin von dem das singnal ausgeht
        self.echo=echo #pin auf em das singnal rein kommt
        
        GPIO.setup(self.trigger, GPIO.OUT)
        GPIO.setup(self.echo, GPIO.IN)
        
    def get_durschnitliche_distanz(self, anz_messungen=int()): #gibt die durschnitliche disanz in mm aus 3 mal messen wieder
        durschnit=float()
        for x in range(anz_messungen):
            durschnit+=self.get_distanz()
            
        durschnit/=anz_messungen
        return durschnit # output ist der dueschnit aus der anzahl der messungen
    
    def get_distanz(self):
        #variablen
        start_time=float()
        stop_time=float()
        
        GPIO.output(self.trigger, True)#kurzens singnal von 0,01ms
        time.sleep(0.00001)
        GPIO.output(self.trigger, False)
        
        while GPIO.input(self.echo)==0: #start timer
            start_time=time.time
            
        while GPIO.input(self.echo)==1: #stop timer
            stop_time=time.time
            
        time_diferenz = stop_time- start_time
        distanc=(time_diferenz*34300)/2
            
        return distanc #output noch unklar muss getestet werden in welcher einheit das ist

if __name__ == '__main__':
    run=True
    
    #AS = Abstandssensor
    AS_vor_rechs=ULTRAschall(trigger=int(), echo=int())
    AS_vor_links=ULTRAschall(trigger=int(), echo=int())
    AS_hinten_rechs=ULTRAschall(trigger=int(), echo=int())
    AS_hinten_links=ULTRAschall(trigger=int(), echo=int())
    
    R_Rechts=Reifen(pin=int())
    R_Links=Reifen(pin=int())
    
    while run:
        #ein durschlauf=1fp
        #'event loop'
        pass