project(qtdlib C CXX)
cmake_minimum_required(VERSION 3.0.0)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(Qt5Core)
find_package(Qt5Qml)
find_package(Qt5Quick)
find_package(Qt5Concurrent)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(QTDLIB "QTdlib")

set(
    SRC
    auth/qtdauthstate.cpp
    auth/qtdauthcode.cpp
    auth/qtdauthstatefactory.cpp
    auth/qtdauthmanager.cpp
    auth/qtdauthparams.cpp
    auth/requests/qtdauthparametersresponse.cpp
    auth/requests/qtdauthencryptionkeyresponse.cpp
    auth/requests/qtdauthphonenumberresponse.cpp
    auth/requests/qtdauthcoderesponse.cpp
    auth/requests/qtdauthpasswordresponse.cpp
    auth/requests/qtdauthlogoutresponse.cpp
    auth/requests/qtdauthdeleteaccountresponse.cpp
    call/qtdcalldiscardreason.cpp
    chat/qtdchat.cpp
    chat/qtdchataction.cpp
    chat/qtdchatactionfactory.cpp
    chat/qtdchatmemberstatus.cpp
    chat/qtdbasicgroupchat.cpp
    chat/qtdsecretchat.cpp
    chat/qtdsecretchatstate.cpp
    chat/qtdsupergroupchat.cpp
    chat/qtdchattype.cpp
    chat/qtdchattypefactory.cpp
    chat/qtdchatlistmodel.cpp
    chat/qtdchatmember.cpp
    chat/qtdchatlistsortfiltermodel.cpp
    chat/requests/qtdgetbasicgrouprequest.cpp
    chat/requests/qtdgetchatsrequest.cpp
    chat/requests/qtdsendchatactionrequest.cpp
    chat/requests/qtdreportchatrequest.cpp
    chat/requests/qtdgetsecretchatrequest.cpp
    chat/requests/qtdgetsupergrouprequest.cpp
    chat/requests/qtdgetsupergroupfullinforequest.cpp
    chat/requests/qtdopenchatrequest.cpp
    chat/requests/qtdclosechatrequest.cpp
    chat/requests/qtdsetpinnedchatsrequest.cpp
    chat/requests/qtdsetchattitlerequest.cpp
    common/qabstracttdobject.cpp
    common/qabstractint53id.cpp
    common/qabstractint32id.cpp
    common/qabstractint64id.cpp
    common/qtdint.cpp
    common/qtdrequest.cpp
    common/qtdresponse.cpp
    common/qtdhelpers.cpp
    client/qtdhandle.cpp
    client/qtdclient.cpp
    client/qtdthread.cpp
    connections/qtdconnectionstate.cpp
    connections/qtdconnectionstatefactory.cpp
    files/qtdfile.cpp
    files/qtdaudio.cpp
    files/qtdlocalfile.cpp
    files/qtdremotefile.cpp
    files/qtddownloadfilerequest.cpp
    files/qtddocument.cpp
    files/qtdphoto.cpp
    files/qtdphotos.cpp
    files/qtdphotosize.cpp
    files/qtdanimation.cpp
    files/qtdsticker.cpp
    files/qtdvideo.cpp
    messages/content/qtdtextentity.cpp
    messages/content/qtdtextentitytype.cpp
    messages/content/qtdmessagedate.cpp
    messages/content/qtdmessagetext.cpp
    messages/content/qtdmessagesticker.cpp
    messages/content/qtdmessagephoto.cpp
    messages/content/qtdmessageanimation.cpp
    messages/content/qtdmessagevideo.cpp
    messages/content/qtdmessageaudio.cpp
    messages/content/qtdmessagedocument.cpp
    messages/content/qtdmessagebasicgroupchatcreate.cpp
    messages/content/qtdmessagecall.cpp
    messages/content/qtdmessagechataddmembers.cpp
    messages/content/qtdmessagechatchangephoto.cpp
    messages/content/qtdmessagechangechattitle.cpp
    messages/content/qtdmessagechatdeletemember.cpp
    messages/content/qtdmessagechatdeletephoto.cpp
    messages/content/qtdmessagechatjoinbylink.cpp
    messages/content/qtdmessagechatsetttl.cpp
    messages/content/qtdmessagechatupgradefrom.cpp
    messages/content/qtdmessagechatupgradeto.cpp
    # todo -----
#    messages/content/qtdmessagecontact.cpp
#    messages/content/qtdmessagecontactregistered.cpp
#    messages/content/qtdmessagecustomserviceaction.cpp
#    messages/content/qtdmessageexpiredphoto.cpp
#    messages/content/qtdmessageexpiredvideo.cpp
#    messages/content/qtdmessagegame.cpp
#    messages/content/qtdmessagegamescore.cpp
#    messages/content/qtdmessageinvoice.cpp
#    messages/content/qtdmessagelocation.cpp
#    messages/content/qtdmessagepassportdatareceived.cpp
#    messages/content/qtdmessagepassportdatasent.cpp
#    messages/content/qtdmessagepaymentsuccessful.cpp
#    messages/content/qtdmessagepaymentsuccessfulbot.cpp
#    messages/content/qtdmessagepinmessage.cpp
#    messages/content/qtdmessagescreenshottaken.cpp
#    messages/content/qtdmessagesupergroupchatcreate.cpp
#    messages/content/qtdmessageunsupported.cpp
#    messages/content/qtdmessagevenue.cpp
#    messages/content/qtdmessagevideonote.cpp
#    messages/content/qtdmessagevoicenote.cpp
#    messages/content/qtdmessagewebsiteconnected.cpp
    # end todo ----
    messages/content/qtdformattedtext.cpp
    messages/content/qtdwebpage.cpp
    messages/qtdmessagecontent.cpp
    messages/qtdmessage.cpp
    messages/qtdmessagecontentfactory.cpp
    messages/qtdmessagesendingstate.cpp
    messages/qtdmessagelistmodel.cpp
    messages/qtdchatstate.cpp
    messages/requests/qtdsendmessagerequest.cpp
    messages/requests/qtdeditmessagetextrequest.cpp
    messages/requests/qtdeditmessagecaptionrequest.cpp
    messages/requests/qtdviewmessagesrequest.cpp
    models/QmlObjectListModel.cpp
    models/QmlVariantListModel.cpp
    network/qtdnetworktype.cpp
    network/requests/qtdsetnetworktype.cpp
    notifications/qtdnotificationsettings.cpp
    notifications/qtdenablenotifications.cpp
    notifications/qtdregisterdevicerequest.cpp
    user/qtduser.cpp
    user/qtdusers.cpp
    user/qtdlinkstate.cpp
    user/qtdlinkstatefactory.cpp
    user/qtduserstatus.cpp
    user/qtdusertype.cpp
    user/qtduserstatusfactory.cpp
    user/qtdprofilephoto.cpp
    user/requests/qtdblockuserrequest.cpp
    user/requests/qtdgetuserrequest.cpp
    utils/qtdtextformatter.cpp
    quick/plugin.cpp
    quick/users.cpp
    chat/requests/qtdleavechatrequest.cpp
    chat/requests/qtddeletechathistoryrequest.cpp
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_library(${QTDLIB} SHARED ${SRC})
link_directories(${CURRENT_TDLIB_BUILD_DIR}/)
include_directories(../vendor/tdlib/)
include_directories(${CURRENT_TDLIB_BUILD_DIR}/)
include_directories(${CURRENT_TDLIB_BUILD_DIR}/generate)
target_link_libraries(${QTDLIB} Qt5::Core Qt5::Quick Qt5::Qml Qt5::Concurrent tdjson)
target_include_directories(${QTDLIB} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
get_filename_component(QTDLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
#set_property(TARGET ${QTDLIB} PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${QTDLIB_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

execute_process(
    COMMAND dpkg-architecture -qDEB_HOST_MULTIARCH
    OUTPUT_VARIABLE ARCH_TRIPLET
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(QT_IMPORTS_DIR "/lib/${ARCH_TRIPLET}")

install(TARGETS ${PLUGIN} DESTINATION ${QT_IMPORTS_DIR})
