<?php

	function __generate_sc_manifest($object_pid, $from_drush = FALSE, $trial = FALSE) {

		require_once ("SharedCanvasManifest.inc");
		require_once ("Canvas.inc");
		require_once ("utlfedoraobject.inc");

		global $base_url;
		$islandora_sc_manifest_image_uri = variable_get('islandora_sc_manifest_image_uri', 'https://iiif.library.utoronto.ca/image/v2/'); 
		$islandora_sc_manifest_presentation_uri = variable_get('islandora_sc_manifest_image_uri', 'https://iiif.library.utoronto.ca/presentation/v2/'); 

		$user = user_load(1);
		$fo = new UTLFedoraObject($user->name, $user->pass);

		$object = $fo->get_object($object_pid);

		$manifest = new SharedCanvasManifest($islandora_sc_manifest_presentation_uri.$object_pid.'/manifest.json', $object->label, "$base_url/islandora/object/$object_pid/datastream/MODS");
		
		// Get the pages ordered by sequence number
		module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
		$pages = islandora_paged_content_get_pages($object);

		if ($from_drush) {
			echo "Processing $object_pid\n";
		}
		foreach($pages as $page) {

		    $page_pid = $page['pid'];

		    if ($from_drush) {
		    	echo "Processing Page: $page_pid\n";
			}

		    $page_object = $fo->get_object($page_pid);		    
		    $page_object_rels = $page_object['JP2']->relationships;
		    $width_rel = $page_object_rels->get('http://islandora.ca/ontology/relsext#', 'width');
		    $height_rel = $page_object_rels->get('http://islandora.ca/ontology/relsext#', 'height');
		    $width = $width_rel[0]['object']['value'];
		    $height = $height_rel[0]['object']['value'];

		    // this width and height is for the original JP2
		    // recalculate width and height based on fixed height of 2000 (which is the case for JPGHIRES
		    //$height = round($width * 2000/$height);
		    //$width = 2000;
		    
		    //$mimetype = $page_object[$dsid]->mimetype;
            $mimetype = "image/jpg";
		    
		    //echo $mimetype . "\n";
		    //echo $width . "\n";
		    //echo $height . "\n";
		    //https://iiif.library.utoronto.ca/v2/paleography%3A2071/full/2000,/0/default.jpg

			$canvas_uri = $islandora_sc_manifest_presentation_uri."$object_pid/canvas/$page_pid";
			//$thumbnail_uri = $page_uri . "/datastream/TN/view";
			$thumbnail_uri = $islandora_sc_manifest_image_uri."$page_pid/full/80,/0/default.jpg";
			$resource_uri = $islandora_sc_manifest_image_uri."$page_pid/full/full/0/default.jpg";
			$service_uri = $islandora_sc_manifest_image_uri."$page_pid";

			$canvas = new Canvas($canvas_uri, $page_object->label, $thumbnail_uri);
			$canvas->setResource($resource_uri, $service_uri, $mimetype, $width, $height);
			$manifest->addCanvas($canvas);

		    //print_r($canvas);
		    //break;
		}
		
		$json = $manifest->getJson();

	    if ($trial) {
	        echo json_encode(json_decode($json), JSON_PRETTY_PRINT) . "\n";
	    }else {
	        // For some reason, the previous SC datastream needs to be purged first. Otherwise add_datasteam() create multiple SC datastreams
	        $object->purgeDatastream('SC');

	        /// Create new datastream only if it doesn't already exist (or if the force flag is set)
	        $fo->add_datastream($object, "SC", "SharedCanvas Manifest", "application/json", $json);
	    }

		drupal_set_message(t('SC Manifest created'));
	}
