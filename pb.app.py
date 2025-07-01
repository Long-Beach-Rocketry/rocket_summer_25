from PyQt6.QtWidgets import * #* imports all the modules from PyQt6.QtWidgets  
from PyQt6.QtGui import * 
from PyQt6.QtCore import *
from PyQt6.QtSerialPort import *


'''
CREATING A CLASS

class MyClass: #MyClass is the name of the class being created
    species = "Canine" #attribute = shared by all instance of the class and defined outside class body
    def __init__(self, name, age): #init is a special method (constructor) that initializes all the parameters (instance attributes) to MyClass
        self.name = name
        self.age = age
    #first parameter of any class is self (refers to instance of class created or being operated on)
    def bark(self): #creating a method called bark
        print(f"{self.name} says woof!") #f" inserts anything { } into the string 
    
my_Dog = MyClass("Laney", "15") #creating an object (my_Dog) with the parameters of MyClass
print(my_Dog.name) #accessing the instance attribute
#will print "Laney"
print(MyClass.species) #accessing the class attribute
#will print "Canine" 
my_Dog.bark() #calls on the bark method
#will print the print statement in bark
'''
'''
QComboBox
METHODS
- addItem() #to add an item
- addItems() #to add items
- clear()
- count() # number of items in collection
- currentText() #text of currently selected item
- itemText() #displays text to specific index
- currentIndex() #index of selected item
- setItemText() #change text of specified index

SIGNALS
- activated() #user chooses item
- currentIndexChanged() #current index changed by user or programnatically
- highlighted() #item in list is highlighted
'''
#Creating Combo Box
app = QApplication([]) #need to instance function correctly
available = QSerialPortInfo.availablePorts() #checking all available ports
x=0 #to start for loop from 0
names = [] #empty list for port names to append to 
for port_info in available: #iterates through all ports
    names.append(port_info.portName())  #appending the port names into the list names
print(names) #printing a list of available port names
class Window(QMainWindow): #creating a class with inheritance for MainWindow to gain features of QMainWindow and have new features once created within the class
    def __init__(self): #initializing with self as parameter (always needs to be the first parameter)
        super().__init__() #for the child class to invoke the constructor of its parent class since QMainWindow is the parent class
        self.setGeometry(200, 100, 800, 800) #setting the size of the window 
        #(x, y, width, height) - x and y are the positional parameters
        self.combo = QComboBox(self) #creating a combo object with the QComboBox functions (dropdown menu)
        for x in range(len(names)): #for loop to iterate through the list of possible ports
            self.combo.addItem(names[x]) #adds the name of each port to the dropdown menu
        self.combo.activated.connect(self.activated_on) #if the user has pressed the buttom
    def activated_on(self):
        current = self.combo.currentText() 
        print(current)
window = Window()
window.show()
app.exec()


