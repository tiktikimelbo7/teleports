# Changelog
All notable changes to this project will be documented in this file.

## 0.6.1
### Bugfix
- A logrotate issue again for the pushhelper. Lets hope its the last one!

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
