Qt SDK Repo Chooser
===================

End-of-life notice
------------------

**The Qt SDK Repo Chooser is now obsolete because Qt IFW 4.0.1-1 now officially supports mirror selection.
Please use that instead: https://wiki.qt.io/Online_Installer_4.x#Selecting_a_mirror_for_opensource**

Qt SDK Repo Chooser has also been unusable since Qt IFW 3.0.2 which
[changed its internals](https://bugreports.qt.io/browse/QTIFW-441?focusedCommentId=372969&page=com.atlassian.jira.plugin.system.issuetabpanels%3Acomment-tabpanel#comment-372969)
to only use server-provided URLs.

This project is now archived. 

Original Project Description
----------------------------
If you are using the online [Qt 5 SDK](http://www.qt.io/download-open-source/) but
downloads are slow and/or you frequently get disconnected, this tool might help.
You might have been assigned a mirror that is performing poorly. The Qt SDK Repo
Chooser helps you select a non-default mirror, which might give you a better
download experience.

Ideally, the causes of this issue should be solved upstream. Please add your
thoughts, ideas, and votes to the reports at [QTIFW-5](https://bugreports.qt.io/browse/QTIFW-5)
and [QTIFW-441](https://bugreports.qt.io/browse/QTIFW-441). In the
meantime though, I hope this tool is useful for you to get started with Qt.


Instructions
------------
See [doc/index.html](http://htmlpreview.github.io/?https://github.com/JKSH/QtSdkRepoChooser/blob/master/doc/index.html).


Alternative Instructions
------------------------
If you can't use this tool for whatever reason, you can still set up your local
repository manually.

1. Download the contents of the following folder and all its subfolders. Ensure
   that you preserve the folder hierarchy:
    - http://download.qt.io/online/qtsdkrepository/{platform}/root/qt/
    - {platform} is one of:
        - "linux_x64"
        - "linux_x86"
        - "mac_x64"
        - "windows_x86"

2. Follow steps #1 - #3 in [doc/index.html](http://htmlpreview.github.io/?https://github.com/JKSH/QtSdkRepoChooser/blob/master/doc/index.html).

3. See the URL of the online installer from the mirror. It should be in this form:
    - {base_path}/official_releases/online_installers/{installer_executable}

4. Open Updates.xml that you downloaded from step #1. Notice a list of URLs in
   this form:
    - http://download.qt.io/online/qtsdkrepository/{platform}/{package}

5. Replace all instances of "http://download.qt.io" with the value of
   {base_path} from step #3.

6. Save Updates.xml.

7. Follow steps #7 - #10 in doc/index.html. The folder that contains Updates.xml
   is your User Defined Repository.


Acknowledgements
----------------
I would like to thank [Samuel Gaist](http://forum.qt.io/user/sgaist) for
providing the prebuilt macOS binaries.
