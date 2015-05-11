
include RbConfig

case CONFIG['host_os']
when /mswin|windows|mingw32/i
    $HOST_OS = "windows"
when /darwin/i
    $HOST_OS = "darwin"
else
    abort("Unknown host config: Config::CONFIG['host_os']: #{Config::CONFIG['host_os']}")
end

$RAKE_ROOT = File.dirname(__FILE__)

ARTIFACTS_FOLDER = "#{$RAKE_ROOT}/Artifacts"
BUILD_FOLDER = "#{$RAKE_ROOT}/Build"

ATOMICTILED_BUILD_DIR = "#{ARTIFACTS_FOLDER}/AtomicTiled_Build"
ATOMICTILED_SOURCE_DIR =  "#{$RAKE_ROOT}/../AtomicTiled"


if $HOST_OS == "darwin"
  $QT_BIN_DIR = "#{ENV["QT_SDK"]}/bin"
else
  $QT_BIN_DIR = "C:\\Qt\\5.4\\msvc2013_64\\bin"
  QT_CREATOR_BIN_DIR = "C:\\Qt\\Tools\\QtCreator\\bin"
  ENV['PATH'] = "#{QT_CREATOR_BIN_DIR};" + ENV['PATH']
end


namespace :android  do

  CMAKE_ANDROID_BUILD_FOLDER = "#{ARTIFACTS_FOLDER}/Android_Build"

  task :player => "macosx:jsbind" do

    if !Dir.exists?("#{CMAKE_ANDROID_BUILD_FOLDER}")
      FileUtils.mkdir_p(CMAKE_ANDROID_BUILD_FOLDER)
    end

    Dir.chdir(CMAKE_ANDROID_BUILD_FOLDER) do

      sh "#{BUILD_FOLDER}/JSBind #{$RAKE_ROOT} ANDROID"
      sh "cmake -DCMAKE_TOOLCHAIN_FILE=#{$RAKE_ROOT}/CMake/Toolchains/android.toolchain.cmake -DCMAKE_BUILD_TYPE=Release ../../"
      sh "make -j8"
    end

  end

end

namespace :ios do

  CMAKE_IOS_BUILD_FOLDER = "#{ARTIFACTS_FOLDER}/IOS_Build"

  task :player => "macosx:jsbind" do

    if !Dir.exists?("#{CMAKE_IOS_BUILD_FOLDER}")
      FileUtils.mkdir_p(CMAKE_IOS_BUILD_FOLDER)
    end

    Dir.chdir(CMAKE_IOS_BUILD_FOLDER) do
      sh "#{BUILD_FOLDER}/JSBind #{$RAKE_ROOT} IOS"
      sh "cmake -DIOS=1 -DCMAKE_BUILD_TYPE=Release -G Xcode ../../"
      sh "xcodebuild -configuration Release"
    end

  end

end


namespace :web do

  CMAKE_WEB_BUILD_FOLDER = "#{ARTIFACTS_FOLDER}/Web_Build"

  task :player => "macosx:jsbind" do

    if !Dir.exists?("#{CMAKE_WEB_BUILD_FOLDER}")
      FileUtils.mkdir_p(CMAKE_WEB_BUILD_FOLDER)
    end

    Dir.chdir(CMAKE_WEB_BUILD_FOLDER) do
      sh "#{BUILD_FOLDER}/JSBind #{$RAKE_ROOT} WEB"
      sh "cmake -DEMSCRIPTEN=1 -DATOMIC_BUILD_2D=1 -DCMAKE_TOOLCHAIN_FILE=#{$RAKE_ROOT}/CMake/Toolchains/emscripten.toolchain.cmake -DCMAKE_BUILD_TYPE=Release ../../"
      sh "make -j8"
    end

    Dir.chdir("#{CMAKE_WEB_BUILD_FOLDER}/Source/AtomicPlayer") do
      sh "mv AtomicPlayer AtomicPlayer.bc"
      sh "emcc -O3 -s ASM_JS=1 -s VERBOSE=0 -s USE_SDL=2 -s TOTAL_MEMORY=134217728 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 -s ERROR_ON_UNDEFINED_SYMBOLS=1 -s NO_EXIT_RUNTIME=1 ./AtomicPlayer.bc -o  ./AtomicPlayer.html"
    end

  end

end


namespace :macosx do


  CMAKE_MACOSX_BUILD_FOLDER = "#{ARTIFACTS_FOLDER}/MacOSX_Build"
  MACOSX_PACKAGE_FOLDER = "#{ARTIFACTS_FOLDER}/MacOSX_Package"

  task :clean do

    folders = ["#{CMAKE_MACOSX_BUILD_FOLDER}", "#{MACOSX_PACKAGE_FOLDER}",
               "#{ARTIFACTS_FOLDER}/Android_Build", "#{ARTIFACTS_FOLDER}/Web_Build",
               "#{ARTIFACTS_FOLDER}/AtomicExamples", "#{ARTIFACTS_FOLDER}/Docs",
               "#{ARTIFACTS_FOLDER}/Examples",  "#{ARTIFACTS_FOLDER}/AtomicTiled_Build",
               "#{ARTIFACTS_FOLDER}/IOS_Build", "#{ARTIFACTS_FOLDER}/IOSDeploy_Build"]

    for index in 0 ... folders.size

        if Dir.exists?(folders[index])
            puts "rm -rf #{folders[index]}"
            sh "rm -rf #{folders[index]}"
        end

        if Dir.exists?(folders[index])
            abort("Unable to clean #{folders[index]}")
        end

    end

  end

	task :cmake do

    FileUtils.mkdir_p(CMAKE_MACOSX_BUILD_FOLDER)

    Dir.chdir(CMAKE_MACOSX_BUILD_FOLDER) do
      sh "cmake ../../ -DCMAKE_BUILD_TYPE=Release"
    end

	end

  task :jsbind => "macosx:cmake" do

    Dir.chdir(CMAKE_MACOSX_BUILD_FOLDER) do
      sh "make -j8 JSBind"
      sh "cp ./Source/AtomicJS/JSBind/JSBind #{BUILD_FOLDER}/JSBind"
    end

  end

	task :generate_javascript_bindings => "macosx:jsbind" do

    Dir.chdir(CMAKE_MACOSX_BUILD_FOLDER) do
      sh "#{BUILD_FOLDER}/JSBind #{$RAKE_ROOT} MACOSX"
    end

	end

  task :generate_docs => "macosx:generate_javascript_bindings" do

    Dir.chdir("#{$RAKE_ROOT}/Docs") do

      if Dir.exists?("out")
        sh "rm -rf out"
      end

      # add the generated JS bindings
      sh "./gendocs.sh"
      sh "cp -r out #{ARTIFACTS_FOLDER}/Docs"
    end

  end

  task :generate_examples do

    Dir.chdir("#{ARTIFACTS_FOLDER}") do

      if Dir.exists?("AtomicExamples")
        sh "rm -rf AtomicExamples"
      end

      if Dir.exists?("Examples")
        sh "rm -rf Examples"
      end

      sh "mkdir Examples"

      sh "git clone https://github.com/AtomicGameEngine/AtomicExamples"

      Dir.chdir("AtomicExamples") do
        sh "git archive master | tar -x -C #{ARTIFACTS_FOLDER}/Examples"
      end

    end

  end

	task :player => "macosx:generate_javascript_bindings" do

    Dir.chdir(CMAKE_MACOSX_BUILD_FOLDER) do
      # add the generated JS bindings
      sh "cmake ../../ -DCMAKE_BUILD_TYPE=Release"
      sh "make -j8 AtomicPlayer"
    end

	end

  task :editor => ["macosx:player"] do

    Dir.chdir(CMAKE_MACOSX_BUILD_FOLDER) do

      sh "make -j8 BuildEditorFiles"

    end

  end

  task :atomictool => ["macosx:player", "macosx:editor"] do

    Dir.chdir(CMAKE_MACOSX_BUILD_FOLDER) do

      sh "make -j8 AtomicTool"

    end

  end

end

namespace :package do

  task :macosx_preflight => ['macosx:clean',
                          'iosdeploy',
                          'web:player',
                          'android:player',
                          'ios:player',
                          "atomictiled:osx",
                          'macosx:editor',
                          'macosx:atomictool',
                          'macosx:generate_examples',
                          'macosx:generate_docs'] do

      FileUtils.mkdir_p(MACOSX_PACKAGE_FOLDER)

      MAC_PLAYER_APP_FOLDER_SRC = "#{CMAKE_MACOSX_BUILD_FOLDER}/Source/AtomicPlayer/AtomicPlayer.app"
      MAC_EDITOR_APP_FOLDER_SRC = "#{CMAKE_MACOSX_BUILD_FOLDER}/Source/AtomicEditor/AtomicEditor.app"

      # Resources
      COREDATA_FOLDER_SRC = "#{$RAKE_ROOT}/Data/AtomicPlayer/Resources/CoreData"
      EDITORAPPLICATIONDATA_FOLDER_SRC = "#{$RAKE_ROOT}/Data/AtomicEditor"

      # Project Templates
      PROJECTTEMPLATES_FOLDER_SRC = "#{EDITORAPPLICATIONDATA_FOLDER_SRC}/ProjectTemplates"

      #Examples
      #Example info could possibly go in the AtomicExamples repo
      EXAMPLEINFO_FOLDER_SRC = "#{EDITORAPPLICATIONDATA_FOLDER_SRC}/ExampleInfo"
      EXAMPLES_FOLDER_SRC = "#{ARTIFACTS_FOLDER}/Examples"

      #Docs
      DOCS_FOLDER_SRC = "#{ARTIFACTS_FOLDER}/Docs"

      MAC_EDITOR_APP_FOLDER_DST = "#{MACOSX_PACKAGE_FOLDER}/AtomicEditor.app"
      MAC_EDITOR_APP_RESOURCE_FOLDER_DST = "#{MACOSX_PACKAGE_FOLDER}/AtomicEditor.app/Contents/Resources"

      # Copy the Editor application
      sh "cp -r #{MAC_EDITOR_APP_FOLDER_SRC} #{MACOSX_PACKAGE_FOLDER}/AtomicEditor.app"

      DEPLOYMENT_FOLDER_DST = "#{MAC_EDITOR_APP_RESOURCE_FOLDER_DST}/Deployment"

      # Make Deployment folder
      FileUtils.mkdir_p("#{DEPLOYMENT_FOLDER_DST}")

      # Copy Resources
      sh "cp -r #{COREDATA_FOLDER_SRC} #{MAC_EDITOR_APP_RESOURCE_FOLDER_DST}/CoreData"

      # Copy Deployment

      # MacOS deployment
      FileUtils.mkdir_p("#{DEPLOYMENT_FOLDER_DST}/MacOS")
      sh "cp -r #{MAC_PLAYER_APP_FOLDER_SRC} #{DEPLOYMENT_FOLDER_DST}/MacOS/AtomicPlayer.app"

      # Android Deployment
      sh "cp -r #{EDITORAPPLICATIONDATA_FOLDER_SRC}/Deployment/Android #{DEPLOYMENT_FOLDER_DST}/Android"
      FileUtils.mkdir_p("#{DEPLOYMENT_FOLDER_DST}/Android/libs/armeabi-v7a")
      sh "cp #{CMAKE_ANDROID_BUILD_FOLDER}/Source/AtomicPlayer/libAtomicPlayer.so #{DEPLOYMENT_FOLDER_DST}/Android/libs/armeabi-v7a/libAtomicPlayer.so"

      # iOS Deployment
      FileUtils.mkdir_p("#{DEPLOYMENT_FOLDER_DST}/IOS/AtomicPlayer.app")
      sh "cp #{CMAKE_IOS_BUILD_FOLDER}/Source/AtomicPlayer/Release-iphoneos/AtomicPlayer.app/AtomicPlayer #{DEPLOYMENT_FOLDER_DST}/IOS/AtomicPlayer.app/AtomicPlayer"
      sh "cp #{CMAKE_IOS_BUILD_FOLDER}/Source/AtomicPlayer/Release-iphoneos/AtomicPlayer.app/PkgInfo #{DEPLOYMENT_FOLDER_DST}/IOS/AtomicPlayer.app/PkgInfo"

      # Web Deployment
      sh "cp -r #{EDITORAPPLICATIONDATA_FOLDER_SRC}/Deployment/Web #{DEPLOYMENT_FOLDER_DST}/Web"
      sh "cp #{CMAKE_WEB_BUILD_FOLDER}/Source/AtomicPlayer/AtomicPlayer.js #{DEPLOYMENT_FOLDER_DST}/Web/AtomicPlayer.js"

      sh "cp -r #{EXAMPLES_FOLDER_SRC} #{MAC_EDITOR_APP_RESOURCE_FOLDER_DST}/Examples"
      sh "cp -r #{DOCS_FOLDER_SRC} #{MAC_EDITOR_APP_RESOURCE_FOLDER_DST}/Docs"

      sh "cp -r #{PROJECTTEMPLATES_FOLDER_SRC} #{MAC_EDITOR_APP_RESOURCE_FOLDER_DST}/ProjectTemplates"
      sh "cp -r #{EXAMPLEINFO_FOLDER_SRC} #{MAC_EDITOR_APP_RESOURCE_FOLDER_DST}/ExampleInfo"

      # DEPLOY TILED
      APPLICATIONS_FOLDER_DST = "#{MAC_EDITOR_APP_FOLDER_DST}/Contents/Applications"

      FileUtils.mkdir_p(APPLICATIONS_FOLDER_DST)

      FileUtils.cp_r("#{ATOMICTILED_BUILD_DIR}/bin/Tiled.app", "#{APPLICATIONS_FOLDER_DST}/Tiled.app")

      Dir.chdir(APPLICATIONS_FOLDER_DST) do
        sh "#{$QT_BIN_DIR}/macdeployqt #{APPLICATIONS_FOLDER_DST}/Tiled.app"
      end

      FileUtils.mkdir_p("#{APPLICATIONS_FOLDER_DST}/CommandLine")

      FileUtils.cp("#{CMAKE_IOSDEPLOY_BUILD_FOLDER}/ios-deploy", "#{APPLICATIONS_FOLDER_DST}/CommandLine/ios-deploy")

      FileUtils.cp("#{CMAKE_MACOSX_BUILD_FOLDER}/Source/AtomicTool/AtomicTool", "#{APPLICATIONS_FOLDER_DST}/CommandLine/AtomicTool")

  end

  task :macosx_editor do

    EDITOR_APP_FOLDER = "#{MACOSX_PACKAGE_FOLDER}/AtomicEditor.app"
    if (!File.file?("#{EDITOR_APP_FOLDER}/Contents/Resources/Deployment/Win64/AtomicPlayer.exe"))
      abort("Missing Windows player, please run rake package:windows_editor from Windows")
    end

    Dir.chdir("#{MACOSX_PACKAGE_FOLDER}") do

      if (File.file?("AtomicEditor_MacOSX.zip"))
        sh "rm AtomicEditor_MacOSX.zip"
      end

      if (File.file?("AtomicEditor_MacOSX.zip"))
        abort ("Couldn't remove editor")
      end

      sh "zip -r AtomicEditor_MacOSX.zip ./AtomicEditor.app"
      sh "zip -T AtomicEditor_MacOSX.zip"

    end

  end


  task :windows_preflight => ['windows:clean',
                              'windows:editor',
                              'windows:atomictool',
                              'atomictiled:windows' ] do


    ATOMICBUILDBOX_SOURCE_DIR =  "#{$RAKE_ROOT}/../AtomicBuildBox/Windows/x64"
    EDITOR_APP_FOLDER_DST = "#{WINDOWS_PACKAGE_FOLDER}/AtomicEditor"

    FileUtils.mkdir_p(EDITOR_APP_FOLDER_DST)

    PLAYER_APP_EXE_SRC = "#{CMAKE_WINDOWS_BUILD_FOLDER}/Source/AtomicPlayer/AtomicPlayer.exe"
    EDITOR_APP_EXE_SRC = "#{CMAKE_WINDOWS_BUILD_FOLDER}/Source/AtomicEditor/AtomicEditor.exe"

    DEPLOYMENT_FOLDER = "#{EDITOR_APP_FOLDER_DST}/Deployment/Win64"

    # Resources
    COREDATA_FOLDER_SRC = "#{$RAKE_ROOT}/Data/AtomicPlayer/Resources/CoreData"
    EDITORAPPLICATIONDATA_FOLDER_SRC = "#{$RAKE_ROOT}/Data/AtomicEditor"

    # Project Templates
    PROJECTTEMPLATES_FOLDER_SRC = "#{EDITORAPPLICATIONDATA_FOLDER_SRC}/ProjectTemplates"

    #Examples
    #Example info could possibly go in the AtomicExamples repo
    EXAMPLEINFO_FOLDER_SRC = "#{EDITORAPPLICATIONDATA_FOLDER_SRC}/ExampleInfo"

    # This shouldn't ne in root, used for deployment atm, however the editor can use coredata from pak
    FileUtils.cp_r("#{COREDATA_FOLDER_SRC}", "#{EDITOR_APP_FOLDER_DST}/CoreData")

    FileUtils.cp("#{CMAKE_WINDOWS_BUILD_FOLDER}/Source/AtomicEditor/EditorData.pak", "#{EDITOR_APP_FOLDER_DST}/EditorData.pak")
    FileUtils.cp("#{CMAKE_WINDOWS_BUILD_FOLDER}/Source/AtomicEditor/CoreData.pak", "#{EDITOR_APP_FOLDER_DST}/CoreData.pak")

    FileUtils.cp_r("#{PROJECTTEMPLATES_FOLDER_SRC}", "#{EDITOR_APP_FOLDER_DST}/ProjectTemplates")
    FileUtils.cp_r("#{EXAMPLEINFO_FOLDER_SRC}", "#{EDITOR_APP_FOLDER_DST}/ExampleInfo")

    FileUtils.mkdir_p("#{DEPLOYMENT_FOLDER}")
    FileUtils.cp("#{ATOMICBUILDBOX_SOURCE_DIR}/D3DCompiler_47.dll", "#{DEPLOYMENT_FOLDER}/D3DCompiler_47.dll")
    FileUtils.cp("#{PLAYER_APP_EXE_SRC}", "#{DEPLOYMENT_FOLDER}/AtomicPlayer.exe")
    FileUtils.cp("#{EDITOR_APP_EXE_SRC}", "#{EDITOR_APP_FOLDER_DST}/AtomicEditor.exe")
    FileUtils.cp("#{ATOMICBUILDBOX_SOURCE_DIR}/D3DCompiler_47.dll", "#{EDITOR_APP_FOLDER_DST}/D3DCompiler_47.dll")

    # DEPLOY TILED
    ATOMICTILED_DEPLOYED_DIR = "#{EDITOR_APP_FOLDER_DST}/Applications/AtomicTiled"

    FileUtils.mkdir_p(ATOMICTILED_DEPLOYED_DIR)

    FileUtils.cp("#{ATOMICTILED_BUILD_DIR}/tiled.exe", "#{ATOMICTILED_DEPLOYED_DIR}")
    FileUtils.cp("#{ATOMICTILED_BUILD_DIR}/tiled.dll", "#{ATOMICTILED_DEPLOYED_DIR}")

    ENV['PATH'] = "#{$QT_BIN_DIR};" + ENV['PATH']
    Dir.chdir(ATOMICTILED_DEPLOYED_DIR) do
      sh "windeployqt.exe --release #{ATOMICTILED_DEPLOYED_DIR}/tiled.exe"
      FileUtils.cp("#{ATOMICBUILDBOX_SOURCE_DIR}/msvcp120.dll", "#{ATOMICTILED_DEPLOYED_DIR}/msvcp120.dll")
      FileUtils.cp("#{ATOMICBUILDBOX_SOURCE_DIR}/msvcr120.dll", "#{ATOMICTILED_DEPLOYED_DIR}/msvcr120.dll")
      FileUtils.cp("#{ATOMICBUILDBOX_SOURCE_DIR}/vccorlib120.dll", "#{ATOMICTILED_DEPLOYED_DIR}/vccorlib120.dll")
    end

    FileUtils.mkdir_p("#{EDITOR_APP_FOLDER_DST}/Applications/CommandLine")
    FileUtils.cp("#{CMAKE_WINDOWS_BUILD_FOLDER}/Source/AtomicTool/AtomicTool.exe", "#{EDITOR_APP_FOLDER_DST}/Applications/CommandLine/AtomicTool.exe")

  end

  task :windows_editor do

    MAC_ARTIFACTS_SRC = "Z:/Artifacts/MacOSX_Package/AtomicEditor.app/Contents/Resources"
    EDITOR_APP_FOLDER_DST = "#{WINDOWS_PACKAGE_FOLDER}/AtomicEditor"

     Dir.chdir("#{WINDOWS_PACKAGE_FOLDER}") do

      if (File.exists?("AtomicEditor_Windows.zip"))
        FileUtils.rm("AtomicEditor_Windows.zip")
      end

      if (File.exists?("AtomicEditor_Windows.zip"))
        abort("Unable to remove AtomicEditor_Windows.zip")
      end

    end

    DEPLOYMENT_FOLDER = "#{EDITOR_APP_FOLDER_DST}/Deployment"
    FileUtils.cp_r("#{MAC_ARTIFACTS_SRC}/Deployment/Android", "#{EDITOR_APP_FOLDER_DST}/Deployment/Android")
    FileUtils.cp_r("#{MAC_ARTIFACTS_SRC}/Deployment/MacOS", "#{EDITOR_APP_FOLDER_DST}/Deployment/MacOS")
    FileUtils.cp_r("#{MAC_ARTIFACTS_SRC}/Deployment/Web", "#{EDITOR_APP_FOLDER_DST}/Deployment/Web")

    FileUtils.cp_r("#{MAC_ARTIFACTS_SRC}/Docs", "#{EDITOR_APP_FOLDER_DST}/Docs")
    FileUtils.cp_r("#{MAC_ARTIFACTS_SRC}/Examples", "#{EDITOR_APP_FOLDER_DST}/Examples")

    # copy windows player to mac
    ATOMICBUILDBOX_SOURCE_DIR =  "#{$RAKE_ROOT}/../AtomicBuildBox/Windows/x64"
    PLAYER_APP_EXE_SRC = "#{CMAKE_WINDOWS_BUILD_FOLDER}/Source/AtomicPlayer/AtomicPlayer.exe"
    FileUtils.mkdir_p("#{MAC_ARTIFACTS_SRC}/Deployment/Win64")
    FileUtils.cp("#{PLAYER_APP_EXE_SRC}", "#{MAC_ARTIFACTS_SRC}/Deployment/Win64/AtomicPlayer.exe")
    FileUtils.cp("#{ATOMICBUILDBOX_SOURCE_DIR}/D3DCompiler_47.dll", "#{MAC_ARTIFACTS_SRC}/Deployment/Win64/D3DCompiler_47.dll")

    Dir.chdir("#{WINDOWS_PACKAGE_FOLDER}") do

      sh "\"C:\\Program Files\\7-Zip\\7z.exe\" a -tzip AtomicEditor_Windows.zip AtomicEditor"
      sh "\"C:\\Program Files\\7-Zip\\7z.exe\" t AtomicEditor_Windows.zip"

    end

  end


end

namespace :windows do

  CMAKE_WINDOWS_BUILD_FOLDER = "#{ARTIFACTS_FOLDER}/Windows_Build"
  WINDOWS_PACKAGE_FOLDER = "#{ARTIFACTS_FOLDER}/Windows_Package"

  task :clean do


    folders = ["#{CMAKE_WINDOWS_BUILD_FOLDER}", "#{WINDOWS_PACKAGE_FOLDER}",
               "#{ARTIFACTS_FOLDER}/AtomicExamples", "#{ARTIFACTS_FOLDER}/Docs",
               "#{ARTIFACTS_FOLDER}/Examples",  "#{ARTIFACTS_FOLDER}/AtomicTiled_Build"]

    for index in 0 ... folders.size

        if Dir.exists?(folders[index])
            FileUtils.rmtree(folders[index])
        end

        if Dir.exists?(folders[index])
            abort("Unable to clean #{folders[index]}")
        end

    end

  end

	task :cmake do

    FileUtils.mkdir_p(CMAKE_WINDOWS_BUILD_FOLDER)

    Dir.chdir(CMAKE_WINDOWS_BUILD_FOLDER) do
      sh "cmake ../../ -G\"NMake Makefiles JOM\" -DCMAKE_BUILD_TYPE=Release"
    end

	end

	task :generate_javascript_bindings => "windows:cmake" do

    Dir.chdir(CMAKE_WINDOWS_BUILD_FOLDER) do
      sh "jom -j4 JSBind"
      sh "./Source/AtomicJS/JSBind/JSBind.exe #{$RAKE_ROOT} WINDOWS"
    end

	end

	task :player => "windows:generate_javascript_bindings" do

    Dir.chdir(CMAKE_WINDOWS_BUILD_FOLDER) do
      # add the generated JS bindings
      sh "cmake ../../ -G\"NMake Makefiles JOM\" -DCMAKE_BUILD_TYPE=Release"
      sh "jom -j4 AtomicPlayer"
    end

	end

  task :editor => "windows:player" do

    Dir.chdir(CMAKE_WINDOWS_BUILD_FOLDER) do

      sh "jom -j4 AtomicEditor"
      sh "jom BuildEditorFiles"

    end

  end

  task :atomictool => "windows:editor" do

    Dir.chdir(CMAKE_WINDOWS_BUILD_FOLDER) do

      sh "jom -j4 AtomicTool"

    end

  end

end

task :iosdeploy do

  CMAKE_IOSDEPLOY_BUILD_FOLDER = "#{ARTIFACTS_FOLDER}/ios-deploy"

  if Dir.exists?(CMAKE_IOSDEPLOY_BUILD_FOLDER)
      FileUtils.rmtree(CMAKE_IOSDEPLOY_BUILD_FOLDER)
  end

  Dir.chdir("#{ARTIFACTS_FOLDER}") do

    sh "git clone https://github.com/AtomicGameEngine/ios-deploy"

    Dir.chdir("ios-deploy") do
      sh "make"
    end

  end

end

namespace :atomictiled do

  task :windows do

    ENV['PATH'] = "#{$QT_BIN_DIR};" + ENV['PATH']

    FileUtils.mkdir_p(ATOMICTILED_BUILD_DIR)

    Dir.chdir(ATOMICTILED_BUILD_DIR) do
      sh "qmake.exe -r \"#{ATOMICTILED_SOURCE_DIR}\\tiled.pro\" \"CONFIG+=release\" \"QMAKE_CXXFLAGS+=-DBUILD_INFO_VERSION=ATOMIC_BUILD\""
      sh "#{QT_CREATOR_BIN_DIR}\\jom.exe -j8"
    end

  end

    task :osx do

      FileUtils.mkdir_p(ATOMICTILED_BUILD_DIR)

      Dir.chdir(ATOMICTILED_BUILD_DIR) do
        sh "#{$QT_BIN_DIR}/qmake -r \"#{ATOMICTILED_SOURCE_DIR}/tiled.pro\" \"CONFIG+=release\" \"QMAKE_CXXFLAGS+=-DBUILD_INFO_VERSION=ATOMIC_BUILD\""
        sh "make -j8"
      end

  end


end
