# coding=utf-8
# Script to upload files to Dropbox

import base64
import sys

from temboo.core.session import TembooSession

from temboo.Library.Dropbox.FilesAndMetadata import UploadFile

print str(sys.argv[1])

with open(str(sys.argv[1]),"rb") as image_file:
    encoded_string = base64.b64encode(image_file.read())


session = TembooSession('iotgroup12','myFirstApp','RxG9n8kuFuSMjtwRvtCvDNnjLDeftlhc')
uploadFileChoreo = UploadFile(session)

uploadFileInputs= uploadFileChoreo.new_input_set()

uploadFileInputs.set_AppSecret("8yponmlcuus4gs3")
uploadFileInputs.set_AccessToken("jvu896qen0p85k3c")
uploadFileInputs.set_FileName(str(sys.argv[1]))
uploadFileInputs.set_AccessTokenSecret("zwdkw2r71i70if2")
uploadFileInputs.set_AppKey("dra3qas1owawxmr")
uploadFileInputs.set_FileContents(encoded_string)
uploadFileInputs.set_Root("sandbox")

uploadFileResults = uploadFileChoreo.execute_with_results(uploadFileInputs)
