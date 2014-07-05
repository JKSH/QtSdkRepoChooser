Qt SDK Repo Chooser
===================
If you are using the online [Qt 5 SDK](http://www.qt-project.org/downloads) but
downloads are slow and/or you frequently get disconnected, this tool might help.
You might have been assigned a mirror that is performing poorly. The Qt SDK Repo
Chooser helps you select a non-default mirror, which might give you a better
download experience.

Ideally, the causes of this issue should be solved upstream. Please add your
thoughts, ideas, and votes to the reports at [QTIFW-5](https://bugreports.qt-project.org/browse/QTIFW-5)
and [QTIFW-441](https://bugreports.qt-project.org/browse/QTIFW-441). In the
meantime though, I hope this tool is useful for you to get started with Qt.


Instructions
------------
See [UserManual.pdf](https://github.com/JKSH/QtSdkRepoChooser/blob/master/UserManual.pdf)


Alternative Instructions
------------------------
If you can't use this tool for whatever reason, you can still set up your local
repository manually.

1. Download the contents of the following folder and all its subfolders. Ensure
   that you preserve the folder hierarchy:
    - http://download.qt-project.org/online/qtsdkrepository/{platform}/root/qt/
    - {platform} is one of:
        - "linux_x64"
        - "linux_x86"
        - "mac_x64"
        - "windows_x86"

2. Follow steps #1 and #2 in [UserManual.pdf]
   (https://github.com/JKSH/QtSdkRepoChooser/blob/master/UserManual.pdf)

3. See the URL of the online installer from the mirror. It should be in this form:
    - {base_path}/official_releases/online_installers/{installer_executable}

4. Open Updates.xml that you downloaded from step #1. Notice a list of URLs in
   this form:
    - http://download.qt-project.org/online/qtsdkrepository/{platform}/{package}

5. Replace all instances of "http://download.qt-project.org" with the value of
   {base_path} from step #3.

6. Save Updates.xml.

7. Follow steps #6 - #9 in UserManual.pdf. The folder that contains Updates.xml
   is your User Defined Repository.
