from PyQt6.QtWidgets import * #* imports all the modules from PyQt6.QtWidgets  
from PyQt6.QtGui import * 
from PyQt6.QtCore import *
from PyQt6.QtSerialPort import *
import time

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
        rows = len(insert) #the amount of rows relies on the length of the list 

#creating first column for controls
        self.columns = QTableWidget(self) #creating self.columns to be the table widget (allows for rows and columns and changing them with textboxes)
        self.columns.resize(300,300) #resizing the columns to visualize all without a scroll bar
        self.columns.setColumnCount(1) #one column 
        self.columns.setRowCount(rows) #setting the amount of rows
        for i in range(rows): #for loop to add items from the list into the table
            item = QTableWidgetItem(insert[i]) #creates the widget item 
            self.columns.setItem(i, 0, item) #adding the item 
        self.columns.itemChanged.connect(self.changed) #if the item in the textbox is changed, refer to changed()
#creating for changed controls
        self.new = QTableWidget(self) #for the new list
        self.new.setGeometry(300, 0, 300,300)
        self.new.setColumnCount(1)
        self.new.setRowCount(rows)
#creating telemetry column
        self.telemetry = QTableWidget(self)
        self.telemetry.setColumnCount(1)
        self.telemetry.setRowCount(len(insert))
        self.telemetry.setEditTriggers(QAbstractItemView.EditTrigger.NoEditTriggers) #making telemetry noneditable
        running = ["0"]
        run = QTableWidgetItem(running[0])
        self.telemetry.setItem(0, 0, run)
#creating tabs 
        self.tab = QTabWidget(self) #creating tabs to switch between
        self.tab.resize(800,800)
        #creating first tab 
        self.tab_one = QWidget() #creating widget to add columns 
        self.box_one = QVBoxLayout() #creating the layout
        self.box_one.addWidget(self.telemetry) #adding telemetry 
        self.box_one.addWidget(self.columns, 100) #adding controls 
        self.tab_one.setLayout(self.box_one) #connecting the widget to layout
        #creating second tab (new controls)
        self.tab_two = QWidget()
        self.box_two = QVBoxLayout()
        self.box_two.addWidget(self.new)
        self.tab_two.setLayout(self.box_two)
        self.tab.addTab(self.tab_one, "first") #creating first tab
        self.tab.addTab(self.tab_two, "second") #creating second tab
#creating a button
        self.button = QPushButton(self) #creating a push button
        self.button.setGeometry(160,270,50,50) #setting the size and location of the button
#if item is changed
    def changed(self):
        for i in range(len(insert)): #for loop to iterate through the rows
            name = self.columns.item(i,0) #grabbing the item in the specific row
            named = name.text() #retrieving the text from the specific row
            new_list.append(named) #adding the text to a new list called new_list
        self.button.clicked.connect(self.buttoned) #if the button is clicked, refer to buttoned()
#if button is clicked
    def buttoned(self):
        #start = time.time()
        running = ["0", "1"] #to display running if 0, once done, displays 1 -- for telemetry
        run = QTableWidgetItem(running[0])
        time.sleep(3)
        self.telemetry.setItem(0, 0, run)
        for i in range(len(insert)): #iterating through all the rows
            itm = QTableWidgetItem(new_list[i]) #creating a new item with each text of the previous table 
            oldItem = QTableWidgetItem(new_list[i]) #creating another new item with each text of previous table to update old table
            self.new.setItem(i, 0, itm) #setting the new text to it's corresponding location
            self.columns.setItem(i, 0, oldItem) #setting the new text to the corresponding location of the old one
        run = QTableWidgetItem(running[1])
        self.telemetry.setItem(0, 0, run)
        '''end = time.time()
        elapsed = end-start
        elap = [elapsed]
        elapse = QTableWidgetItem(elapsed[0])
        self.telemetry.setItem(1, 0, elapse)'''
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
    
