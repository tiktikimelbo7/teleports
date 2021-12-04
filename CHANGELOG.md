# Changelog
All notable changes to this project will be documented in this file.

## 0.12

### Changed
- Upgraded tdlib to unpublished version 1.7.9 due to massive 1.6.0 API breakage by Telegram
- Check complete date when showing date change markers in the message list (Bjarne Roß)
- Different palette for the avatar background color if no profile pic is present

### Bugfix
- Audio message content displays correctly even if some details of the audio clip are missing

## 0.11
### Added
- Added button to copy group or username shortcut from profile to clipboard
- Added support for animated stickers, finally (playback speed will be improved)!
- The ESC key is now a shortcut to go back, improves keyboard usage

### Changed
- A lot of translations have been updated, thanks!

### Bugfix
- Ghost notifications on the launcher icon (emblem counter) should be gone
- When forwarding chats the chat list to select the target is visible again
- embedded URLs with a link text were not clickable
- reworked how "Forwarded from:" info is built, this should prevent empty and missing chats
- Clicking on an empty picture in profiles does not open media viewer
- Pictures in various places were not displayed instantly when coming from new messages
- Stickers loaded for the first time were also not always shown immediately

## 0.10
### Added
- Chat filters for title and type of chat
- Add handling of archived chats
- Contacts import via Contacts list

### Bugfix
- Various fixes for initial chat loading
- Try to utilize more threads and prevent deadlocks on opening the app
- New messages in the chat that was left just before are no longer marked as read
- Notifications can now be properly turned on and off also from TELEports
- Fixed translations on the password page and other layout issues in the login workflow
- Stop certain animations playing in the background to improve CPU resource usage (fixed in 0.10.1)

## 0.9
### Added
- Implemented sending of voice notes (tap and hold the mike!)
- Implemented joining a chat by public or invite link

### Changed
- A lot of translations have been updated, thanks!
- Pinned message action is shown correctly
- All action text is now resized and multi-line to allow loooong user names ;)

### Bugfix
- TELEports icon in system settings is now visible
- Show chat photo change or removal
- Fixed the attachment bar not to overlap the message area in some cases

## 0.8
### Added
- Option to turn notifications on and off per group or user
- Fastscroll and sorting added to contact list. When starting to scroll the letters can be clicked to jump
- Added a new option to also show single, double check marks and clock icon as message status indicators. How retro!
- Incoming messages can now be displayed with underlined and strike-through formatting

### Changed
- Upgrade to tdlib 1.6 - Thank You Telegram!
- Caption of all possible message types can now be edited
- User and group info have been unified to use the same UI elements, secret chat key fingerprint relocated

### Bugfix
- Replying to a message with a sticker works now
- Pinephone users s could not see the message input box
- Forwarding messages does not delete any draft in the same chat
- Formatting of code was inconsistent when used with 2 backticks
- Message draft remembers also if its a reply to another message
- Incoming voice note recording info does not crash the App anymore
- Audio files could not be shared to other apps

## 0.7
### Added
- TELEports is now available also for 64bit devices
- Secret chats: Show fingerprint picture in chat details
- Draft support: load and store drafts when leaving a chat or suspend the App
- Show sender name, message and thumbnail when replying/editing a message
- Red background color in case a message failed to send
- Stickers: Basic selection of stickers for sending from the new stickerpanel
- Notifications: Mute/unmute chat notifications from chat details (user mute will follow)
- Thumbnail preview for documents when available
- Added Slovak and Norwegian languages

### Bugfix
- Summary texts in the chatlist for member actions were messed up
- Various issues with sizing the message bubbles
- Performance bugs due to excessive logging
- Channel messages do not overflow the bottom info box
- Non-square stickers are no longer distorted
- Buttons in popup dialogs are now at the end of the box as expected
- Incoming/Outgoing call info was swapped in some cases

## 0.6
### Bugfix
- Disabled account deletion due to reports of unwanted deleted accounts
- Secret chats with negative IDs were set to read-only

### Added
- Button to return to the last message when viewing older messages ("Flying Button") - thx Krille!
- Userprofile shows now more details (bio, number of groups in common)
- Notification counter emblem on the icon now shows better values, notifications should disappear when messages are read on any device
- Content sharing works now (incoming share requests from e.g. Gallery or file manager)
- Attach more than one file
- Contacts page! View your contacts, add, remove, edit, jump to chat with a person, start new secret chat

### Changed
- User joined/Left messages are now working as they should
- During registration input boxes accept keyboard press on Enter key
- Auto-download is now disabled for all attachments except pictures. This saves network bandwidth
- Add online member count to group header
- Show group details also when viewing them from the chat list

## 0.5
### Added
- Message text can be copied to clipboard via left-swipe
- New language: Dutch
- Location sending is now also live, missed the last release
- Location maps are now styled better in size and appearance
- Audio and video files can be attached via attach panel

### Changed
- Lots of graphical improvements!
- Upgrade to tdlib 1.4 - Thank You Telegram
- Do not show text input box in channels and terminated secret chats
- The registration and login process now shows sensible error messages instead of just being quiet

## 0.4.1
### Bugfix
- A logrotate issue surfaced when we suddenly received errors from the push helper. Provide bandaid to not fill people´s log directories anymore

## 0.4
### Added
- Finally: Unread Messages indicator in message list and positioning there!
- If other people edit or delete messages in an open chat TELEports will now update the message list instantly
- View counts in a channel will be updated if the channel is open
- Start of Dutch translation
- Support for video and voice notes
- Secret chats show if a screenshot has been taken
- Push notifications are clickable and should open the correct chat
- Push notifications show the chat avatar
- Push notifications get cleared when messages are read on another device

### Changed
- Sorting/display of chats was sometimes broken due to a wrong loading sequence
- Some strings in the chat list and user status were never translated
- Various status messages in the message list have been corrected
- Join and leave messages show if the user did this action or a moderator/admin

## 0.3
### Added
- Forwarding of messages finally here. Also display forwarded messages properly
- Location attachments can be seen as minimap - Sending of location comes next release
- Network status indicator (ping-pong-bar) is active whenever there is a connection problem
- Connectivity status page in the App menu
- Mentions are displayed in chat list with @ symbol
### Changed
- Chat list: Shows now the real actions that users do: Sent a picture, document etc. - Not just "Sent a message"
- Popup dialogs have right Suru colors
- Code input for registration is now numbers only
- Documents and files can now be attached properly
- Stickers have a transparent background
- Message size limit: mesages longer than 4096 characters or so are now split up into multiple messages. Go type!
