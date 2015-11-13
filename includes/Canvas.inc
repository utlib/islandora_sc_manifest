<?php
class Canvas {
  protected $object_id;
  protected $object_label;
  protected $metadata_uri;
  protected $image_uri;
  protected $image_format;
  protected $image_height;
  protected $image_width;

  function __construct($id, $label, $meta_uri) {
    $this->object_id = $id;
    $this->object_label = $label;
    $this->metadata_uri = $meta_uri;
  }

  function setImage($uri, $format = 'image/jpeg', $width, $height) {
    $this->image_uri = $uri;
    $this->image_format = $format;
    $this->image_width = $width;
    $this->image_height = $height;
  }

  function toArray() {
    $returnArray = array (
        '@type' => 'sc:Canvas',
        '@id' => $this->object_id,
        'label' => $this->object_label,
        'metadata' => $this->metadata_uri,
        'images' => array ( // always contain only one
            array (
                '@type' => 'oa:Annotation',
                'motivation' => 'sc:Painting',
                'resource' => array (
                    '@id' => $this->image_uri,
                    '@type' => 'dctypes:Image',
                    'format' => $this->image_format,
                    'height' => $this->image_height,
                    'width' => $this->image_width
                )
            )
        )
    );

    return $returnArray;
  }
}
