<?php
class SharedCanvasManifest {
  protected $id = '';
  protected $object_label = '';
  protected $metadata_uri = '';
  protected $canvases = array ();

  /**
   * Initiate the manifest
   *
   * @param unknown $metadata_uri
   * @param string $object_label
   */
  function __construct($id, $label, $meta_uri) {
    $this->id = $id;
    $this->object_label = $label;
    $this->metadata_uri = $meta_uri;
  }

  /**
   * Add a $canvas to $sequences
   *
   * @param string $canvas
   */
  function addCanvas($canvas = null) {
    if ($canvas != null) {
      $this->canvases [] = $canvas->toArray();
    }
  }

  /**
   * Build and return a json string based on what we have in the class
   */
  function getJson() {
    $scManifest = array (
        '@id' => $this->id,
        '@type' => 'sc:Manifest',
        'label' => $this->object_label,
        'metadata' => $this->metadata_uri,
        'sequences' => array ( // an array but will always be a single object in our application
            array (
                '@type' => 'sc:Sequence',
                'label' => $this->object_label . ', in order',
                'canvases' => $this->canvases
            )
        )
    );
    return json_encode ( $scManifest );
  }
}
