# Changelog
All notable changes to this project will be documented in this file.

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