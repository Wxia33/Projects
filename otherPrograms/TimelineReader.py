import twitter
import json
import StringIO
api = twitter.Api(
    consumer_key = 'ScmAh44We5SfoQmlZqj7rUbzR',
    consumer_secret = 'v8EPw3p3EtwXwsl3Qyay5s0FH6T29ZqrYuUdUyouaLPU0CXkVs',
    access_token_key = '553219518-IP9baQ3hGuqw43J8DQO1WrxbAB4jgyfriM3lIlao',
    access_token_secret = 'vAuIyqrSzr2GGzL5kxhGfzzG3RTkorWLjQcWqvZZlFocI')
#user = twitter.User(
#    name = "niamhmdouglas")
name = raw_input("Target User: ")
number = int(raw_input("Number of Tweets and Favorites Requested: "))
#name = "niamhmdouglas"
#number = 3
#Extracts Indexed output for User-Friendly Analysis
def extract(data): 
    x = 0
    while x < number:
        print data[x]
        x += 1
         
#Retrieves Timeline (up to 200 recent statuses)
timeline = [s.text for s in api.GetUserTimeline(
    screen_name = name,
    count = number,
    max_id = None)]
print ""
extract(timeline)
print ""

#Retrieves Favorites
favorites = [s.text for s in api.GetFavorites(
    screen_name = name,
    count = number,
    max_id = None)]
extract(favorites)

raw_input()
