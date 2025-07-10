from PyQt6.QtWidgets import * #* imports all the modules from PyQt6.QtWidgets  
from PyQt6.QtGui import * 
from PyQt6.QtCore import *
from PyQt6.QtSerialPort import *
import sys

#class to protodisplay - one parameter(list)
application = QApplication([]) #creating the application to run the window
insert = [] #list for users to insert more
new_list = []
rows = 0 #the amount of rows    
i = 0 #meant for the for loop (in range of rows)
class Window(QMainWindow): #creating window for the Qtable to display (inheritance)
    def __init__(self): #initializing parameters
        super().__init__() #since it's child class, needs this initialization
        self.setGeometry(200, 200, 800, 800) #setting the size of the window
        self.columns = QTableWidget(self) #creating self.columns to be the table widget (allows for rows and columns and changing them with textboxes)
        self.columns.resize(300,300) #resizing the columns to visualize all without a scroll bar
        self.columns.setColumnCount(1) #one column 
        self.new = QTableWidget(self) #for the new list
        self.new.setGeometry(300, 0, 300,300)
        self.new.setColumnCount(1)
        rows = len(insert) #the amount of rows relies on the length of the list 
        self.columns.setRowCount(rows) #setting the amount of rows
        self.new.setRowCount(rows)
        #print(rows) 
        for i in range(rows): #for loop to add items from the list into the table
            item = QTableWidgetItem(insert[i]) #creates the widget item 
            self.columns.setItem(i, 0, item) #adding the item 
        '''if self.columns.event(35): #icon changed 
            self.columns.changeEvent()

            self.columns.setItem()'''
        self.button = QPushButton(self) #creating a push button
        self.button.setGeometry(160,100,50,50) #setting the size and location of the button
        #self.button.clicked.connect(self.apply)
        self.columns.itemChanged.connect(self.changed)
    def changed(self):
        for i in range(len(insert)):
            name = self.columns.item(i,0)
            named = name.text()
            new_list.append(named)
        #self.columns.setItem(self.columns.currentRow(), self.columns.currentColumn(), self.columns.item(self.columns.currentRow(), self.columns.currentColumn()) )
        self.button.clicked.connect(self.buttoned)
    def buttoned(self):
        for i in range(len(insert)):
            itm = QTableWidgetItem(new_list[i])
            oldItem = QTableWidgetItem(new_list[i])
            self.new.setItem(i, 0, itm)
            self.columns.setItem(i, 0, oldItem)
        print(new_list)
class Protodisplay: #creating a class called protodisplay   
    def __init__(self, newlist): #initializing variables (list)
        self.newlist = newlist 
    
    def listed(self): #creates a method 
        for item in self.newlist: #iterating through a list 
            insert.append(item) #adding the new items to the list
        print(insert) #verification for the list

mylist = Protodisplay(["apples", "oranges", "carrots"]) #creating a list with personal parameters
mylist.listed() #calling on listed to add those items to the table
window = Window() 
window.show() #displaying the window
application.exec() #keeping the window up until x is pressed 
    
