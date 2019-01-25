#pragma once

struct ToastPayload
{
	LPCTSTR pszName;
	LPCTSTR pszXML;
};

ToastPayload g_ToastPayloads[] =
{

	{ _T("Basic.xml"),
	_T("<toast>\r\n\
 <visual>\r\n\
  <binding template=\"ToastGeneric\">\r\n\
   <text>Hello World</text>\r\n\
   <text>This is a simple toast message</text>\r\n\
  </binding>\r\n\
 </visual>\r\n\
</toast>")
	},

{ _T("Blank.xml"),
_T("<toast>\r\n\
 <visual>\r\n\
  <binding template=\"ToastGeneric\">\r\n\
  </binding>\r\n\
 </visual>\r\n\
</toast>")
},

{ _T("BreakingNews.xml"),
_T("<toast launch=\"action=viewStory&amp;storyId=92187\">\r\n\
 <visual>\r\n\
  <binding template=\"ToastGeneric\">\r\n\
   <text>Tortoise beats rabbit in epic race</text>\r\n\
   <text>In a surprising turn of events, Rockstar Rabbit took a nasty crash, allowing Thomas the Tortoise to win the race.</text>\r\n\
   <text placement=\"attribution\">The Animal Times</text>\r\n\
  </binding>\r\n\
 </visual>\r\n\
</toast>")
},

{ _T("Alarm.xml"),
_T("<toast launch=\"action=viewAlarm&amp;alarmId=3\" scenario=\"alarm\">\r\n\
 <visual>\r\n\
  <binding template=\"ToastGeneric\">\r\n\
   <text>Time to wake up!</text>\r\n\
   <text>To prove you're awake, select which of the following fruits is yellow...</text>\r\n\
  </binding>\r\n\
 </visual>\r\n\
 <actions>\r\n\
  <input id=\"answer\" type=\"selection\" defaultInput=\"wrongDefault\">\r\n\
   <selection id=\"wrong\" content=\"Orange\"/>\r\n\
   <selection id=\"wrongDefault\" content=\"Blueberry\"/>\r\n\
   <selection id=\"right\" content=\"Banana\"/>\r\n\
   <selection id=\"wrong\" content=\"Avacado\"/>\r\n\
   <selection id=\"wrong\" content=\"Cherry\"/>\r\n\
  </input>\r\n\
 <action arguments=\"snooze\" content=\"Snooze\"/>\r\n\
 <action arguments=\"dismiss\" content=\"Dismiss\"/>\r\n\
 </actions>\r\n\
</toast>")
},

{ _T("EventInvite.xml"),
_T("<toast launch=\"action=viewEvent&amp;eventId=63851\">\r\n\
 <visual>\r\n\
  <binding template=\"ToastGeneric\">\r\n\
   <text>Surface Launch Party</text>\r\n\
   <text>Studio S / Ballroom</text>\r\n\
   <text>4:00 PM, 10 / 26 / 2015</text>\r\n\
  </binding>\r\n\
 </visual>\r\n\
 <actions>\r\n\
  <input id=\"status\" type=\"selection\" defaultInput=\"yes\">\r\n\
   <selection id=\"yes\" content=\"Going\"/>\r\n\
   <selection id=\"maybe\" content=\"Maybe\"/>\r\n\
   <selection id=\"no\" content=\"Decline\"/>\r\n\
  </input>\r\n\
  <action arguments=\"action= rsvpEvent&amp;eventId=63851\" content=\"RSVP\"/>\r\n\
  <action arguments=\"dismiss\" content=\"Dismiss\"/>\r\n\
 </actions>\r\n\
</toast>")
},

{ _T("FriendRequest.xml"),
_T("<toast launch=\"action=viewFriendRequest&amp;userId=49183\">\r\n\
 <visual>\r\n\
  <binding template=\"ToastGeneric\">\r\n\
   <text>Matt sent you a friend request</text>\r\n\
   <text>Hey, wanna dress up as wizards and ride around on our hoverboards together?</text>\r\n\
   <image placement=\"appLogoOverride\" hint-crop=\"circle\" src=\"file://%EXEPATH%\\64.jpg\"/>\r\n\
  </binding>\r\n\
 </visual>\r\n\
 <actions>\r\n\
  <action content=\"Accept\" arguments=\"action=acceptFriendRequest&amp;userId=49183\"/>\r\n\
  <action content=\"Decline\" arguments=\"action=declineFriendRequest&amp;userId=49183\"/>\r\n\
 </actions>\r\n\
</toast>")
},

{ _T("IncomingCall.xml"),
_T("<toast launch=\"action=answer&amp;callId=938163\" scenario=\"incomingCall\">\r\n\
 <visual>\r\n\
  <binding template=\"ToastGeneric\">\r\n\
   <text>Andrew Bares</text>\r\n\
   <text>Incoming Call - Mobile</text>\r\n\
   <image hint-crop=\"circle\" src=\"file://%EXEPATH%\\100.jpg\"/>\r\n\
  </binding>\r\n\
 </visual>\r\n\
 <actions>\r\n\
  <action content=\"Text reply\" imageUri=\"file://%EXEPATH%\\message.png\" arguments=\"action=textReply&amp;callId=938163\"/>\r\n\
  <action content=\"Reminder\"  imageUri=\"file://%EXEPATH%\\reminder.png\" arguments=\"action=reminder&amp;callId=938163\"/>\r\n\
  <action content=\"Ignore\" imageUri=\"file://%EXEPATH%\\cancel.png\" arguments=\"action=ignore&amp;callId=938163\"/>\r\n\
  <action content=\"Answer\"  imageUri=\"file://%EXEPATH%\\telephone.png\" arguments=\"action=answer&amp;callId=938163\"/>\r\n\
 </actions>\r\n\
</toast>")
},

{ _T("Messaging.xml"),
_T("<toast launch=\"action=openThread&amp;threadId=92187\">\r\n\
 <visual>\r\n\
  <binding template=\"ToastGeneric\">\r\n\
   <text hint-maxLines=\"1\">Jill Bender</text>\r\n\
   <text>Check out where we camped last weekend!It was incredible, wish you could have come on the backpacking trip!</text>\r\n\
   <image placement=\"appLogoOverride\" hint-crop=\"circle\" src=\"file://%EXEPATH%\\65.jpg\"/>\r\n\
   <image placement=\"hero\" src=\"file://%EXEPATH%\\180.jpg\"/>\r\n\
  </binding>\r\n\
 </visual>\r\n\
 <actions>\r\n\
  <input id=\"textBox\" type=\"text\" placeHolderContent=\"reply\"/>\r\n\
  <action content=\"Send\" imageUri=\"file://%EXEPATH%\\send.png\" hint-inputId=\"textBox\" arguments=\"action=reply&amp;threadId=92187\"/>\r\n\
 </actions>\r\n\
</toast>")
},

{ _T("ProgressBar.xml"),
_T("<toast launch=\"action=viewDownload&amp;downloadId=9438108\">\r\n\
 <visual>\r\n\
  <binding template=\"ToastGeneric\">\r\n\
   <text>Downloading this week's new music...</text>\r\n\
   <progress title=\"Katy Perry\" value=\"0.35\" valueStringOverride=\"3/15 songs\" status=\"Downloading...\"/>\r\n\
  </binding>\r\n\
 </visual>\r\n\
 <actions>\r\n\
  <action arguments=\"action=pauseDownload&amp;downloadId=9438108\" content=\"Pause\"/>\r\n\
  <action arguments=\"action=cancelDownload&amp;downloadId=9438108\" content=\"Cancel\"/>\r\n\
 </actions>\r\n\
</toast>")
},

{ _T("PhotoTagged.xml"),
_T("<toast launch=\"action=viewPhoto&amp;photoId=92187\">\r\n\
 <visual>\r\n\
  <binding template=\"ToastGeneric\">\r\n\
   <image placement=\"appLogoOverride\" hint-crop=\"circle\" src=\"file://%EXEPATH%\\66.jpg\"/>\r\n\
   <text>Adam Wilson tagged you in a photo</text>\r\n\
   <text>On top of McClellan Butte - with Andrew Bares</text>\r\n\
   <image src=\"file://%EXEPATH%\\202.jpg\"/>\r\n\
  </binding>\r\n\
 </visual>\r\n\
 <actions>\r\n\
  <action content=\"Like\" arguments=\"likePhoto&amp;photoId=92187\"/>\r\n\
  <action content=\"Comment\" arguments=\"action=commentPhoto&amp;photoId=92187\"/>\r\n\
 </actions>\r\n\
</toast>")
},

{ _T("Reminder.xml"),
_T("<toast launch=\"action=viewEvent&amp;eventId=1983\" scenario=\"reminder\">\r\n\
 <visual>\r\n\
  <binding template=\"ToastGeneric\">\r\n\
   <text>Adaptive Tiles Meeting</text>\r\n\
   <text>Conf Room 2001 / Building 135</text >\r\n\
   <text>10:00 AM - 10:30 AM</text>\r\n\
  </binding>\r\n\
 </visual>\r\n\
 <actions>\r\n\
  <input id=\"snoozeTime\" type=\"selection\" defaultInput=\"15\">\r\n\
   <selection id=\"1\" content=\"1 minute\"/>\r\n\
   <selection id=\"15\" content=\"15 minutes\"/>\r\n\
   <selection id=\"60\" content=\"1 hour\"/>\r\n\
   <selection id=\"240\" content=\"4 hours\"/>\r\n\
   <selection id=\"144\" content=\"1 day\"/>\r\n\
  </input>\r\n\
  <action arguments=\"snooze\" hint-inputId=\"snoozeTime\" content=\"Snooze\"/>\r\n\
  <action arguments=\"dismiss\" content=\"Dismiss\"/>\r\n\
 </actions>\r\n\
</toast>")
},

{ _T("Restaurant.xml"),
_T("<toast launch=\"action=viewRestaurant&amp;restaurantId=92187\">\r\n\
 <visual>\r\n\
  <binding template=\"ToastGeneric\">\r\n\
   <image placement=\"hero\" src=\"file://%EXEPATH%\\Food1.jpg\"/>\r\n\
   <text hint-maxLines=\"1\">New suggested restaurant</text>\r\n\
   <text>There's a popular chinese restaurant near you that we think you'd like!</text>\r\n\
   <image src=\"file://%EXEPATH%\\RestaurantMap.jpg\"/>\r\n\
   <group>\r\n\
    <subgroup>\r\n\
     <text hint-style=\"body\">Pho Licious</text>\r\n\
     <text hint-style=\"captionSubtle\">4.6 stars</text>\r\n\
    </subgroup>\r\n\
    <subgroup hint-textStacking=\"bottom\">\r\n\
     <text hint-style=\"captionSubtle\" hint-wrap=\"true\" hint-align=\"right\">4018 148th Ave NE, Redmond, WA 98052</text>\r\n\
    </subgroup>\r\n\
   </group>\r\n\
  </binding>\r\n\
 </visual>\r\n\
 <actions>\r\n\
  <action content=\"Map\" arguments=\"bingmaps:?q=4018 148th Ave NE, Redmond, WA 98052\" activationType=\"protocol\"/>\r\n\
  <action content=\"Reviews\" arguments=\"action=viewRestaurantReviews&amp;restaurantId=92187\"/>\r\n\
 </actions>\r\n\
</toast>")
},

{ _T("Weather.xml"),
_T("<toast>\r\n\
<visual baseUri=\"Assets/Apps/Weather/\">\r\n\
 <binding template=\"ToastGeneric\">\r\n\
  <text>Today will be sunny with a high of 63 and a low of 42.</text>\r\n\
   <group>\r\n\
    <subgroup hint-weight=\"1\">\r\n\
     <text hint-align=\"center\">Mon</text>\r\n\
     <image src=\"file://%EXEPATH%\\Mostly Cloudy.png\" hint-removeMargin=\"true\"/>\r\n\
     <text hint-align=\"center\">63°</text>\r\n\
     <text hint-style=\"captionsubtle\" hint-align=\"center\">42°</text>\r\n\
    </subgroup>\r\n\
    <subgroup hint-weight=\"1\">\r\n\
     <text hint-align=\"center\">Tue</text>\r\n\
     <image src=\"file://%EXEPATH%\\Cloudy.png\" hint-removeMargin=\"true\"/>\r\n\
     <text hint-align=\"center\">57°</text>\r\n\
     <text hint-style=\"captionsubtle\" hint-align=\"center\">38°</text>\r\n\
    </subgroup>\r\n\
    <subgroup hint-weight=\"1\">\r\n\
     <text hint-align=\"center\">Wed</text>\r\n\
     <image src=\"file://%EXEPATH%\\Sunny.png\" hint-removeMargin=\"true\"/>\r\n\
     <text hint-align=\"center\">59°</text>\r\n\
     <text hint-style=\"captionsubtle\" hint-align=\"center\">43°</text>\r\n\
    </subgroup>\r\n\
    <subgroup hint-weight=\"1\">\r\n\
     <text hint-align=\"center\">Thu</text>\r\n\
     <image src=\"file://%EXEPATH%\\Sunny.png\" hint-removeMargin=\"true\"/>\r\n\
     <text hint-align=\"center\">62°</text>\r\n\
     <text hint-style=\"captionsubtle\" hint-align=\"center\">42°</text>\r\n\
    </subgroup>\r\n\
    <subgroup hint-weight=\"1\">\r\n\
     <text hint-align=\"center\">Fri</text>\r\n\
     <image src=\"file://%EXEPATH%\\Sunny.png\" hint-removeMargin=\"true\"/>\r\n\
     <text hint-align=\"center\">71°</text>\r\n\
     <text hint-style=\"captionsubtle\" hint-align=\"center\">66°</text>\r\n\
    </subgroup>\r\n\
   </group>\r\n\
  </binding>\r\n\
 </visual>\r\n\
</toast>")
},

{ _T("Windows 8.1 style toast (ToastText01).xml"),
_T("<toast>\r\n\
 <visual>\r\n\
  <binding template=\"ToastText01\">\r\n\
   <text id=\"1\">bodyText</text>\r\n\
  </binding>\r\n\
 </visual>\r\n\
</toast>")
},

{ _T("Windows 8.1 style toast (ToastImageAndText04).xml"),
_T("<toast>\r\n\
 <visual>\r\n\
  <binding template=\"ToastImageAndText04\">\r\n\
   <image id=\"1\" src=\"file://%EXEPATH%\\IC601609.png\" alt=\"image1\"/>\r\n\
   <text id=\"1\">Lorem upsum dolor sit amet con...</text>\r\n\
   <text id=\"2\">This is the first line</text>\r\n\
   <text id=\"3\">This is the second line</text>\r\n\
  </binding>\r\n\
 </visual>\r\n\
</toast>")
}

};