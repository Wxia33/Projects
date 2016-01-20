# -*- coding: utf-8 -*-
"""
Created on Fri Sep 04 18:58:39 2015

@author: Oracle
"""
import pymongo
from dbco import *
import time

number_articles = db.qdoc.find().count()
