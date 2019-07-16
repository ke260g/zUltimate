'''
AndroidManifest.xml
'''

import sys
import xml.etree.ElementTree as ET

android_namespace = 'http://schemas.android.com/apk/res/android'
android_name = '{' + android_namespace + '}' + 'name'
android_value = '{' + android_namespace + '}' + 'value'
android_glEsVersion = '{' + android_namespace + '}' + 'glEsVersion'

'''
{
    'tag1': -> ET.substance
    'uses-permission': {
        '<android:name>'
        'android.permission.RECEIVE_USER_PRESENT': --> ET.substance
        'android.permission.WRITE_SETTINGS': --> ET.substance
    },
    'uses-feature': {
        'android:glEsVersion'
        '<android:name>': --> ET.substance
    },
    'supports-screens': {
        
    }
}
'''

class AMXml:
    __mETree = None
    __manifest = None       # 节点实体 <manifest />
    __uses_permission = {}  # dict, <key, value> = <android:name, 节点实体>
    __uses_feature = {}     # dict, <key, value> = <android:name, 节点实体>
    __glESVersion = None    # 节点实体
    __uses_sdk = None       # 节点实体
    __application = None
    
    def __init__(self, path):
        ET.register_namespace('android', android_namespace)
        self.__mETree = ET.parse(path)
        # for elem in mTree.iter() 递归遍历
        # for elem in mTree.getroot() 单层遍历
        # mTree.getroot() 理论上为 manifest.xml
        self.__manifest = self.__mETree.getroot()
        if self.__manifest.tag != 'manifest':
            raise ValueError('root of xml is not a <manifest>')

        for attrib in self.__manifest.attrib.items():
            print(attrib)

        for elem in self.__manifest:
            if elem.tag == 'uses-permission':
                permission = elem.attrib.get(android_name)
                if self.__uses_permission.get(permission) == None:
                    self.__uses_permission[permission] = elem
                else:
                    # 删除重复 permission 节点
                    self.__manifest.remove(elem)
            elif elem.tag == 'uses-feature':
                feature = elem.attrib.get(android_name)
                if feature:
                    # <uses-feature android:name />
                    if self.__uses_feature.get(feature) == None:
                        self.__uses_feature[feature] = elem
                    else:
                        # 删除重复 feature 节点
                        self.__manifest.remove(elem)
                else:
                    # <uses-feature android:glEsVersion />
                    glEsVersion = elem.attrib.get(android_glEsVersion)
                    if glEsVersion:
                        self.__glESVersion = elem

    def save(self, path):
        self.__mETree.write(path, 
            encoding='utf-8',
            xml_declaration='<?xml version="1.0" encoding="utf-8" standalone="no"?>')
        pass

    def setOnlyLauncher(self, name):
        pass

    def setVersionCode(self, v):
        self.__manifest.attrib['versionCode'] = str(v)
    
    def getVersionCode(self):
        return self.__manifest.attrib.get('versionCode')

    def setVersionName(self, v):
        self.__manifest.attrib['versionName'] = str(v)
    
    def getVersionName(self):
        return self.__manifest.attrib.get('versionName')
    
    def setPackageName(self, v):
        self.__manifest.attrib['package'] = str(v)

    def getPackageName(self):
        return self.__manifest.attrib.get('package')
    
    def setMinSdkVersion(self, v):
        pass
    
    def getMinSdkVersion(self):
        pass

    def setTargetSdkVersion(self, v):
        pass
    
    def getTargetSdkVersion(self):
        pass

    def setGlEsVersion(self, v):
        if self.__glESVersion != None:
            self.__glESVersion.attrib[android_glEsVersion] = str(v)
        else:
            # 没有就创建
            elem = ET.Element('uses-feature',
	            attrib={ android_glEsVersion: str(v) })
            self.__manifest.insert(0, elem)
            self.__glESVersion = elem

    def getGlEsVersion(self):
        if self.__glESVersion != None:
            return self.__glESVersion.attrib.get(android_glEsVersion)
    
    def delGlEsVersion(self):
        if self.__glESVersion != None:
            self.__manifest.remove(self.__glESVersion)
            self.__glESVersion = None

'''
# example
if __name__ == '__main__':
    mAMXml = AMXml('./AndroidManifest.xml')
    print(mAMXml.getVersionCode())
    mAMXml.setVersionCode('4')

    print(mAMXml.getVersionName())
    mAMXml.setVersionName('10.2.4')

    print(mAMXml.getPackageName())
    mAMXml.setPackageName('com.symphony.composer')

    print(mAMXml.getGlEsVersion())
    mAMXml.delGlEsVersion()
    mAMXml.setGlEsVersion('0x20000')
    mAMXml.setGlEsVersion('0x30000')
    mAMXml.delGlEsVersion()

    mAMXml.save('new.xml')
'''