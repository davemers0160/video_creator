#define _CRT_SECURE_NO_WARNINGS

#if defined(_WIN32) | defined(__WIN32__) | defined(__WIN32) | defined(_WIN64) | defined(__WIN64)
#include <windows.h>
#elif defined(__linux__) 

#endif

#include <cstdint>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
//#include <thread>
//#include <sstream>
//#include <fstream>

#include "file_parser.h"
#include "get_directory_listing.h"

// OpenCV includes
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui/highgui.hpp>     
//#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/videoio.hpp>

//
//// ----------------------------------------------------------------------------------------
//// https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
//std::vector<std::string> get_directory_listing(std::string folder)
//{
//    std::vector<std::string> file_names;
//    std::string search_path = folder + "*.*";
//
//    WIN32_FIND_DATA fd;
//    void* hFind = ::FindFirstFile(search_path.c_str(), &fd);
//
//#if defined(_WIN32) | defined(__WIN32__) | defined(__WIN32) | defined(_WIN64) | defined(__WIN64)
//    if (hFind != INVALID_HANDLE_VALUE)
//    {
//        do 
//        {
//            // read all (real) files in current folder
//            // , delete '!' read other 2 default folder . and ..
//            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
//                file_names.push_back(fd.cFileName);
//            }
//        } while (::FindNextFile(hFind, &fd));
//        ::FindClose(hFind);
//    }
//#else
//    DIR* dir;
//    class dirent* ent;
//    class stat st;
//
//    dir = opendir(directory.c_str());
//    while ((ent = readdir(dir)) != NULL) {
//        const string file_name = ent->d_name;
//        const string full_file_name = directory + "/" + file_name;
//
//        if (file_name[0] == '.')
//            continue;
//
//        if (stat(full_file_name.c_str(), &st) == -1)
//            continue;
//
//        const bool is_directory = (st.st_mode & S_IFDIR) != 0;
//
//        if (is_directory)
//            continue;
//
//        file_names.push_back(file_name); // returns just filename
//    }
//    closedir(dir);
//#endif
//
//    return file_names;
//}

// ----------------------------------------------------------------------------------------
void print_help()
{
	//system("cls");
	//std::cout << "Video Converter" << std::endl;
 //   std::cout << "David Emerson" << std::endl << std::endl;
 //   std::cout << "Developed to convert video files recorded using various codecs to a windows friendly MP4 codec.  ";
 //   std::cout << "No audio is saved in the conversion." << std::endl;
 //   std::cout << "Usage: Video Convert <input_file_name> <width> <height>" << std::endl << std::endl;

 //   std::cout << "Press Enter to Continue..." << std::endl;
	//cin.ignore();
}

// ----------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
    uint64_t idx;
    
    //std::string videofilename;
    std::string video_save_file;
	//cv::VideoCapture video_in;
	cv::VideoWriter output_video;
	cv::Mat input_image;
    //int frameWidth, frameHeight;
    uint32_t img_width, img_height;
    uint64_t frame_count;
    int32_t codec;
    double frame_rate;
    std::string codec_str;
	//int save_codec = CV_FOURCC('F', 'M', 'P', '4');

    std::string save_path;
    std::string input_folder;
    std::vector<std::string> file_list;
    
    std::vector<std::string> filter;

    //	unsigned int img_width = 0;
//	unsigned int img_height = 0;
//	cv::Rect crop;


    const std::string params =
        "{help h ?  | | Help message }"
        "{codec     | MJPG | Video save codec }"
        //"{codec     | FMP4 | Video save codec }"
        "{fps       | 30.0 | Frame rate }"
        "{filter    | .png | Image filter }"
        //"{rx_address  | 10.127.1.101 | IP Address for the lidar to send data to }"
        //"{os1_address | 10.127.1.175 | IP address for the lidar }"
        //"{cfg_port    | 7501 | TCP/IP Port for lidar configuration }"
        //"{lidar_port  | 7502 | UDP Port to receive lidar Stream }"
        //"{imu_port    | 7503 | UDP Port for the IMU data }"
        //"{cfg_file    |  | Alternate input method to supply all parameters, all parameters must be included in the file }"
        //"{avg         | 11 | Number of full lidar captures to average }"
        //"{output      | ../results/ | Output directory to save lidar images }"
        ;

    // use opencv's command line parser
    cv::CommandLineParser parser(argc, argv, params);

    if (parser.has("help"))
    {
        parser.printMessage();
        std::cout << "Press enter to continue..." << std::endl;
        std::cin.ignore();
        return 0;
    }


    // if the input is a config file use this over all other input parameters
    // check to make sure that the parser has an input folder
    //if (parser.has("codec"))
    //{
    //    // input config file should contain all required inputs
    //    std::string cfg_filename = parser.get<std::string>("cfg_file");
    //    std::vector<std::vector<std::string>> cfg_params;
    //    parseCSVFile(cfg_filename, cfg_params);

    //    if (cfg_params.size() == 5)
    //    {
    //        os1_address = cfg_params[0][0];
    //        config_port = std::stoi(cfg_params[1][0]);
    //        lidar_port = std::stoi(cfg_params[1][1]);
    //        imu_port = std::stoi(cfg_params[1][2]);
    //        rx_address = cfg_params[2][0];
    //        capture_num = std::stoi(cfg_params[3][0]);
    //        save_path = cfg_params[4][0];
    //    }
    //    else
    //    {
    //        std::cout << "The number of supplied parameters in the file does not meet the required criteria: N = " << cfg_params.size() << std::endl;
    //        std::cin.ignore();
    //        return -1;
    //    }
    //}
    //else
    //{
    //    rx_address = parser.get<std::string>("rx_address");
    //    os1_address = parser.get<std::string>("os1_address");
    //    config_port = parser.get<int32_t>("cfg_port");
    //    lidar_port = parser.get<int32_t>("lidar_port");
    //    imu_port = parser.get<int32_t>("imu_port");
    //    capture_num = parser.get<uint32_t>("avg");
    //    save_path = parser.get<std::string>("output");
    //}

    input_folder = path_check(std::string(argv[1]));

    frame_rate = parser.get<double>("fps");
    codec_str = parser.get<std::string>("codec");
    filter.clear();
    filter.push_back(parser.get<std::string>("filter"));

    try {

        std::cout << "Processing the following folder: ";
        std::cout << input_folder << std::endl << std::endl; 
        
        file_list = get_directory_listing(input_folder, filter);

        codec = cv::VideoWriter::fourcc((char)codec_str[0], (char)codec_str[1], (char)codec_str[2], (char)codec_str[3]);
               
        video_save_file = input_folder + "video_file.avi";


        if (file_list.size() > 0)
        {
            std:cout << "Found: " << file_list.size() << " images." << std::endl;

            input_image = cv::imread((input_folder + file_list[0]), cv::IMREAD_ANYCOLOR);
            img_width = input_image.cols;
            img_height = input_image.rows;

        }
        else
        {
            return 0;
        }

        std::cout << std::endl << "Saving the following file:" << std::endl;
        std::cout << video_save_file << std::endl;   

        output_video.open(video_save_file, codec, frame_rate, cv::Size(img_width, img_height), true);
        
        std::cout << "FPS:   " << frame_rate << std::endl;
        std::cout << "CODEC: " << codec_str << std::endl;
        std::cout << "Size:  " << img_width << " x " << img_height << std::endl;

        for (idx = 0; idx < file_list.size(); ++idx)
        {
            input_image = cv::imread((input_folder + file_list[idx]), cv::IMREAD_ANYCOLOR);

            if ((input_image.rows != img_height) || (input_image.cols != img_width))
            {
                std::cout << "Current image size does not match the initial video size:" << std::endl;
                std::cout << file_list[idx] << ": " << input_image.cols << " x " << input_image.rows << std::endl;
                continue;
            }
            output_video.write(input_image);
        }

        output_video.release();

        std::cout << std::endl << "Processing complete!" << std::endl;

    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;       
    }

    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.ignore();

	return 1;


}	// end of main





