<?php

/**
 * Creates the administration form 
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the form state.
 *
 * @return array
 *   A renderable array representing the islandora_mirador_bookreader administration form.
 */
function islandora_sc_manifest_admin_settings_form($form, &$form_state) {
  $get_default_value = function($name, $default) use($form_state) {
    return isset($form_state['values'][$name]) ? $form_state['values'][$name] : variable_get($name, $default);
  };
  //$solr_enabled = module_exists('islandora_solr');
  $form = array(
	'islandora_sc_manifest_iiif_server' => array(
		'#type' => 'textfield',
		'#title' => t('IIIF compliant image server base URL'),
		'#description' => t('Base URL of the IIIF image server used to generate absolute IIIF URLs. Add URL without a trailing slash'),
		'#default_value' => $get_default_value('islandora_sc_manifest_iiif_server', ''),
	)
  );
  return system_settings_form($form);
}
