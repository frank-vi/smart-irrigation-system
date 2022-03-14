import sqlite3
import pathlib
import os

class Database():
  def __init__(self):
    """ SQLite Database """

  def connect(self):
    try:
      # WARN: CHANGE PATH WITH YOUR ABSOLUTE PATH      
      self.connection = sqlite3.connect(os.path.join(pathlib.Path.cwd(), '/home/smartcity/project/control-unit-irrigation/irrigation-system/database/gateway.db'))
      self.cursor = self.connection.cursor()
    except:
      print("DB connection timeout!")

  def save_data(self, values):
    self.cursor.execute('INSERT INTO measurement VALUES (?,?,?,?)', values)
    self.connection.commit()

  def read_data(self):
    self.cursor.execute('SELECT * FROM measurement')
    return self.cursor.fetchall()

  def clear(self):
    self.cursor.execute('DELETE FROM measurement')
    self.connection.commit()
    return self.cursor.fetchall()

  def disconnect(self):
    self.connection.close()