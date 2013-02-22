/* interface.cc */

void mesh::interface(){
	bool finished;
	ipoint p;

	finished=false;
	while(finished==false){
		XNextEvent(display, &report);
		switch(report.type) {
			case ButtonPress:
				p=mouse_location();
				break;
			case KeyPress:
				if(XLookupKeysym(&report.xkey, 0) == XK_q){ // quit           
                    finished=true;
                    XCloseDisplay(display);
                    exit(0);
                    break;
                };
                if(XLookupKeysym(&report.xkey, 0) == XK_Left){ // rotate left
                	linear_transformation(YRmat(-0.1));
                	erase_field();
                	draw_mesh();
                	break;
                };
                if(XLookupKeysym(&report.xkey, 0) == XK_Right){	// rotate right
                	linear_transformation(YRmat(0.1));
                	erase_field();
                	draw_mesh();
                	break;
                };
                if(XLookupKeysym(&report.xkey, 0) == XK_Up){	// rotate up
                	linear_transformation(XRmat(-0.1));
                	erase_field();
                	draw_mesh();
                	break;
                };   
                if(XLookupKeysym(&report.xkey, 0) == XK_Down){	// rotate down
                	linear_transformation(XRmat(0.1));
                	erase_field();
                	draw_mesh();
                	break;
                };          
                if(XLookupKeysym(&report.xkey, 0) == XK_equal){	// make bigger
                 	linear_transformation(Dmat(1.05));
                	erase_field();
                	draw_mesh();
                	break;               
                };
                if(XLookupKeysym(&report.xkey, 0) == XK_minus){	// make smaller
                 	linear_transformation(Dmat(1.0/1.05));
                	erase_field();
                	draw_mesh();
                	break;               
                };
                if(XLookupKeysym(&report.xkey, 0) == XK_2){		// make isobars denser
                	iheight=iheight/1.1;
                	erase_field();
                	draw_mesh();
                	break;                 	
                };
                if(XLookupKeysym(&report.xkey, 0) == XK_1){		// make isobars sparser
                	iheight=iheight*1.1;
                	erase_field();
                	draw_mesh();
                	break;                 	
                };
                if(XLookupKeysym(&report.xkey, 0) == XK_f){		// toggle mesh/solid
                	WIRE=!WIRE;
                	erase_field();
                	draw_mesh();
                	break;
                };
                if(XLookupKeysym(&report.xkey, 0) == XK_v){		// toggle verbose on/off
                	verbose=!verbose;
                	break;
                };
                if(XLookupKeysym(&report.xkey, 0) == XK_i){		// toggle isobar/shade
                	isobar=!isobar;
                	erase_field();
                	draw_mesh();
                	break;
                };
                if(XLookupKeysym(&report.xkey, 0) == XK_s){		// subdivide
                	subdivide();
                	erase_field();
                	draw_mesh();
                	break;
                };
                if(XLookupKeysym(&report.xkey, 0) == XK_c){		// curvature flow
                	curvature_flow();
                	erase_field();
                	draw_mesh();
                	break;
                };
                if(XLookupKeysym(&report.xkey, 0) == XK_t){		// trim pyramids
                	trim_pyramids();
                	erase_field();
                	draw_mesh();
                	break;
                };
                if(XLookupKeysym(&report.xkey, 0) == XK_e){		// eps output
                	eps_draw();
                	break;
                };
                if(XLookupKeysym(&report.xkey, 0) == XK_w){		// write mesh output
                	write_mesh_to_file();
                	break;
                };
                if(XLookupKeysym(&report.xkey, 0) == XK_p){		// write povray output
                	write_povray_file();
                	break;
                };
            default:
            	break;
        };
    };
};